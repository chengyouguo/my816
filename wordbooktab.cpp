#include "wordbooktab.h"
#include "ui_wordbooktab.h"
#include "wordmanager.h"
#include "audioplayer.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileInfo>

WordBookTab::WordBookTab(QWidget *parent)
    : BaseTab(parent), ui(new Ui::WordBookTab)
{
    ui->setupUi(this);

    ui->gridLayout->setRowStretch(3, 1);
    ui->gridLayout_3->setRowStretch(0, 1);

    // 表格
    auto *model = new QStandardItemModel(this);
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "单词");
    model->setHeaderData(1, Qt::Horizontal, "音标");
    model->setHeaderData(2, Qt::Horizontal, "释义");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 首次加载
    onGradeChanged(currentGrade());

    // 信号槽
    connect(&AudioPlayer::instance(), &AudioPlayer::playingFinished,
            this, [this]() { ui->btnplayer->setEnabled(true); });
    connect(&AudioPlayer::instance(), &AudioPlayer::errorOccurred,
            this, [this](const QString &word, const QString &error) {
                Q_UNUSED(error);
                ui->btnplayer->setEnabled(true);
                ui->editWord->setText(word);
                emit statusMessage(QString("未获取到音频，请检查单词：%1").arg(word));
                ui->editWord->selectAll();
                ui->editWord->setFocus();
            });

    connect(ui->editWord, &QLineEdit::textChanged,
            this, [this](const QString &t) { if (!t.isEmpty()) on_btnSearch_clicked(); });
    connect(ui->btnSearch,  &QPushButton::clicked, this, &WordBookTab::on_btnSearch_clicked);
    connect(ui->btnAddWord, &QPushButton::clicked, this, &WordBookTab::on_btnAddWord_clicked);
    connect(ui->btnplayer,  &QPushButton::clicked, this, &WordBookTab::on_btnplayer_clicked);

    connect(ui->tableView, &QTableView::clicked,
            this, [this](const QModelIndex &index) {
                if (!index.isValid()) return;
                auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
                if (!model) return;
                QString word = model->item(index.row(), 0)->text();
                ui->editWord->setText(word); // 触发 textChanged → 自动搜索
            });
}

WordBookTab::~WordBookTab()
{
    delete ui;
}

void WordBookTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty())
        return;

    if (ui->lineEditGrade) {
        ui->lineEditGrade->setText(grade);
        ui->lineEditGrade->setReadOnly(true);
    }

    auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (!model) return;
    model->removeRows(0, model->rowCount());

    // ★ QVariantMap 版本
    QVector<QVariantMap> words = manager()->wordsOfGrade(grade);
    for (const auto &w : words) {
        model->appendRow({
            new QStandardItem(w["word"].toString()),
            new QStandardItem(w["phonetic"].toString()),
            new QStandardItem(w["definition"].toString())
        });
    }

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
    QString key = ui->editWord->text().trimmed();
    if (key.isEmpty()) return;

    // ★ 返回 QVariantMap
    QVariantMap w = manager()->findWordInDict(key);
    if (w.isEmpty() || w["word"].toString().isEmpty()) {
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
    QString word = ui->editWord->text().trimmed();
    if (word.isEmpty()) {
        ui->editWord->setFocus();
        return;
    }

    QString grade = currentGrade();
    if (grade.isEmpty()) {
        emit statusMessage("请先在主窗口选择年级");
        return;
    }

    // ★ fuzzyFindWord 返回 QVariantMap
  //  QVariantMap info = manager()->fuzzyFindWord(word);

    bool ok = manager()->addWordToGrade(grade, word);

    if (ok) {
        onGradeChanged(grade);

        // 找到刚加的词，滚过去高亮
        auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
        if (model) {
            for (int i = 0; i < model->rowCount(); ++i) {
                if (model->item(i, 0)->text().compare(word, Qt::CaseInsensitive) == 0) {
                    QModelIndex idx = model->index(i, 0);
                    ui->tableView->scrollTo(idx, QAbstractItemView::PositionAtCenter);
                    ui->tableView->setCurrentIndex(idx);
                    break;
                }
            }
        }

        emit statusMessage(QString("添加成功：%1").arg(word));
    } else {
        // 判断是否重复
        QVector<QVariantMap> gradeWords = manager()->wordsOfGrade(grade);
        int existRow = -1;
        for (int i = 0; i < gradeWords.size(); ++i) {
            if (gradeWords[i]["word"].toString().compare(word, Qt::CaseInsensitive) == 0) {
                existRow = i;
                break;
            }
        }

        if (existRow >= 0) {
            emit statusMessage(QString("已存在：%1（不要重复添加）").arg(word));
            auto *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
            if (model) {
                QModelIndex idx = model->index(existRow, 0);
                ui->tableView->scrollTo(idx, QAbstractItemView::PositionAtCenter);
                ui->tableView->setCurrentIndex(idx);
            }
        } else {
            emit statusMessage(QString("总表无此词，无法添加：%1").arg(word));
        }
    }

    ui->editWord->clear();
    ui->editPhonetic->clear();
    ui->editDef->clear();
    ui->editExample->clear();
    ui->editWord->setFocus();

    AudioPlayer::instance().play(word);
}

void WordBookTab::playWord(const QString &word)
{
    ui->btnplayer->setEnabled(false);
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
    // ★ 避免切页时清空输入框：只在年级真变了才刷新
    static QString lastGrade;
    QString cur = currentGrade();
    if (!cur.isEmpty() && cur != lastGrade) {
        lastGrade = cur;
        onGradeChanged(cur);
    }
}