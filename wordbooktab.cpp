#include "wordbooktab.h"
#include "ui_wordbooktab.h"
#include "wordmanager.h"
#include "audioplayer.h"   // ✅ AudioManager → AudioPlayer

#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileInfo>

WordBookTab::WordBookTab(WordManager *mgr, QWidget *parent)
    : BaseTab(mgr, parent), ui(new Ui::WordBookTab)
{
    ui->setupUi(this);

    // 让表格组占满窗口拉高后的剩余空间
    ui->gridLayout->setRowStretch(3, 1);
    ui->gridLayout_3->setRowStretch(0, 1);

    // 表格初始化
    auto *model = new QStandardItemModel(this);
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "单词");
    model->setHeaderData(1, Qt::Horizontal, "音标");
    model->setHeaderData(2, Qt::Horizontal, "释义");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 年级变化信号（BaseTab 已连接 gradeChanged → onGradeChanged）
    // 首次加载
    onGradeChanged(manager() ? manager()->currentGrade() : QString());

    // 信号槽
    connect(&AudioPlayer::instance(), &AudioPlayer::playingFinished,
            this, [this]() {
                ui->btnplayer->setEnabled(true);
            });

    connect(ui->editWord, &QLineEdit::textChanged,
            this, [this](const QString &t){ if(!t.isEmpty()) on_btnSearch_clicked(); });

    connect(ui->btnSearch,  &QPushButton::clicked, this, &WordBookTab::on_btnSearch_clicked);
    connect(ui->btnAddWord, &QPushButton::clicked, this, &WordBookTab::on_btnAddWord_clicked);
    connect(ui->btnplayer,  &QPushButton::clicked, this, &WordBookTab::on_btnplayer_clicked);

    connect(ui->tableView, &QTableView::clicked,
            this, [this](const QModelIndex &index){
                if (!index.isValid()) return;
                auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
                if (!model) return;
                QString word = model->item(index.row(), 0)->text();
                ui->editWord->setText(word);  // 触发 textChanged → 自动搜索填充
            });
}

WordBookTab::~WordBookTab()
{
    delete ui;
}

void WordBookTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty() || !manager())
        return;

    if (ui->lineEditGrade) {
        ui->lineEditGrade->setText(grade);
        ui->lineEditGrade->setReadOnly(true);
    }

    auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (!model) return;

    model->removeRows(0, model->rowCount());

    QVector<QVariantMap> words = manager()->wordsOfGrade(grade);
    for (const auto &w : words) {
        model->appendRow({
            new QStandardItem(w["word"].toString()),
            new QStandardItem(w["phonetic"].toString()),
            new QStandardItem(w["definition"].toString())
        });
    }

    // 复位输入区
    ui->editWord->clear();
    ui->editPhonetic->clear();
    ui->editDef->clear();
    ui->editExample->clear();
    ui->editWord->setFocus();

    if (ui->statusLabel)
        ui->statusLabel->setText(QString("当前年级：%1 | 共%2个").arg(grade).arg(words.size()));

    emit statusMessage(QString("已加载：%1（%2个）").arg(grade).arg(words.size()));
}

void WordBookTab::on_btnSearch_clicked()
{
    if (!manager()) return;
    QString key = ui->editWord->text().trimmed();
    if (key.isEmpty()) return;

    QVariantMap w = manager()->fuzzyFindWord(key);
    if (w.isEmpty()) {
        ui->editPhonetic->clear();
        ui->editDef->clear();
        ui->editExample->clear();
        emit statusMessage(QString("未找到：%1").arg(key));
        return;
    }

    ui->editPhonetic->setText(w["phonetic"].toString());
    ui->editDef->setText(w["definition"].toString());
    ui->editExample->setText(w["example"].toString());
    emit statusMessage(QString("已找到：%1").arg(w["word"].toString()));
}

void WordBookTab::on_btnAddWord_clicked()
{
    if (ui->editWord->text().isEmpty()) {
        ui->editWord->setFocus();
        return;
    }

    QString grade = manager()->currentGrade();
    if (grade.isEmpty()) {
        emit statusMessage("请先在主窗口选择年级");
        return;
    }

    QString word = ui->editWord->text().trimmed();
    QString phon = ui->editPhonetic->text().trimmed(); (void)phon;
    QString def  = ui->editDef->text().trimmed();      (void)def;
    QString exam = ui->editExample->text().trimmed();  (void)exam;

    // 替换原有的 bool ok = manager()->addWordToGrade(grade, word); 及下方if块
    bool ok = manager()->addWordToGrade(grade, word);
    auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());

    if (ok) {
        if (model) {
            model->appendRow({
                new QStandardItem(word),
                new QStandardItem(phon),
                new QStandardItem(def)
            });

            // 新词：滚到最底并高亮
            QModelIndex lastIndex = model->index(model->rowCount() - 1, 0);
            ui->tableView->scrollTo(lastIndex, QAbstractItemView::PositionAtBottom);
            ui->tableView->setCurrentIndex(lastIndex);
        }
        emit statusMessage(QString("添加成功：%1").arg(word));
    } else {
        // 处理添加失败（重复或总表无词）
        QVector<QVariantMap> gradeWords = manager()->wordsOfGrade(grade);
        bool existsInGrade = false;
        int existRow = -1;

        for (int i = 0; i < gradeWords.size(); ++i) {
            if (gradeWords[i]["word"].toString().compare(word, Qt::CaseInsensitive) == 0) {
                existsInGrade = true;
                existRow = i; // 记录重复单词在模型中的行号
                break;
            }
        }

        if (existsInGrade) {
            emit statusMessage(QString("已存在：%1（未重复添加）").arg(word));
            // ✅ 已存在：滚动到原单词并高亮
            if (model && existRow >= 0 && existRow < model->rowCount()) {
                QModelIndex existIndex = model->index(existRow, 0);
                ui->tableView->scrollTo(existIndex, QAbstractItemView::PositionAtCenter); // 居中显示
                ui->tableView->setCurrentIndex(existIndex);
            }
        } else {
            emit statusMessage(QString("总表无此词，无法添加：%1").arg(word));
        }
    }

    // 清空输入框
    ui->editWord->clear();
    ui->editPhonetic->clear();
    ui->editDef->clear();
    ui->editExample->clear();
    ui->editWord->setFocus();
}


void WordBookTab::playWord(const QString &word)
{
    ui->btnplayer->setEnabled(false);


    // ✅ 全部交给 AudioPlayer：内部自动处理
    //    1. exe/audio/<word>.mp3 存在 -> 直接播
    //    2. 不存在 -> Downloader 下载 -> 播
    //    3. 正在下载 -> 合并请求，不重复下载
    //    4. 坏文件(<=100B) -> 自动删除，下次重试
    //   （缓存目录 = applicationDirPath()/audio，由 AudioPlayer 自己管）
    AudioPlayer::instance().play(word);
    emit statusMessage(QString("播放：%1").arg(word));
}

void WordBookTab::on_btnplayer_clicked()
{
    QString word = ui->editWord->text().trimmed().toLower();
    if (word.isEmpty()) {
        emit statusMessage("请先输入或选中一个单词");
        return;
    }
    playWord(word);
}

void WordBookTab::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (manager())
        onGradeChanged(manager()->currentGrade());
}
