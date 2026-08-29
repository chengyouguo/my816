#include "wordbooktab.h"
#include "ui_wordbooktab.h"
#include "wordmanager.h"
#include "downloader.h"
#include "audiomanager.h"
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QSettings>
#include <QDir>

WordBookTab::WordBookTab(WordManager *mgr, QWidget *parent)
    : BaseTab(mgr, parent), ui(new Ui::WordBookTab)
{
    ui->setupUi(this);
    // 让表格组占满窗口拉高后的剩余空间
    ui->gridLayout->setRowStretch(3, 1);

    // 让表格在 groupTable 内部也占满
    ui->gridLayout_3->setRowStretch(0, 1);
    // ✅ 删掉这两行（BaseTab 已经干了）
    // m_mgr = mgr;
    // connect(m_mgr, &WordManager::gradeChanged, this, &WordBookTab::onGradeChanged);
    // onGradeChanged(m_mgr->currentGrade());

    // 表格初始化
    auto *model = new QStandardItemModel(this);
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "单词");
    model->setHeaderData(1, Qt::Horizontal, "音标");
    model->setHeaderData(2, Qt::Horizontal, "释义");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 信号槽
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

    QVector<QVariantMap> words = manager()->wordsOfGrade(grade); // ✅ 用 manager()
    for (const auto &w : words)
        model->appendRow({
            new QStandardItem(w["word"].toString()),
            new QStandardItem(w["phonetic"].toString()),
            new QStandardItem(w["definition"].toString())
        });
    // ✅ 成功后复位
    ui->editWord->clear();
    ui->editPhonetic->clear();
    ui->editDef->clear();
    ui->editExample->clear();
    ui->editWord->setFocus();  // 焦点回到单词框，直接敲下一个
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
   ui->editExample->setText(w["example"].toString()); // ✅ 正确
    emit statusMessage(QString("已找到：%1").arg(w["word"].toString()));
}

void WordBookTab::on_btnAddWord_clicked()
{   if (ui->editWord->text().isEmpty()) {
         ui->editWord->setFocus();  // 焦点回到单词框，直接敲下一个
       // QMessageBox::warning(this, "提示", "单词不能为空");
        return;
    }
    QString grade = manager()->currentGrade();
    //QString grade = currentGrade(); // ✅ 用基类接口
    if (grade.isEmpty()) {
        emit statusMessage("请先在主窗口选择年级");
        return;
    }

    // ✅ 不再读 comboGrade，直接问 Manager
    // grade = manager()->currentGrade();
  //  if (grade.isEmpty()) {
      //  emit statusMessage("请先在主窗口选择年级");
      //  return;
   // }

    QString word = ui->editWord->text().trimmed();
    QString phon = ui->editPhonetic->text().trimmed();
    QString def  = ui->editDef->text().trimmed();
    QString exam = ui->editExample->text().trimmed();

  //  if (word.isEmpty() || def.isEmpty()) {
   //     QMessageBox::warning(this, "提示", "单词和释义不能为空");
    //    return;
  //  }

    on_btnplayer_clicked();

    bool ok = manager()->addWordToGrade(grade, word);
    if (ok) {
        onGradeChanged(grade); // 刷新当前表格
        emit statusMessage(QString("已添加：%1").arg(word));
    } else {
        QMessageBox::critical(this, "错误", "添加失败");
    }
    }

    void WordBookTab::on_btnplayer_clicked()
{
    if (!manager()) return;
    QString word = ui->editWord->text().trimmed();
    if (word.isEmpty()) return;


    // 获取当前工作目录
          //  QDir currentDir(QDir::currentPath());
//
            // 音频目录：跟 exe 走，和 mypro_data 同级
            QString audioDir = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/audio");
            QDir().mkpath(audioDir);

            QString path = audioDir + "/" + word + ".mp3";

            if (QFile::exists(path)) {
                AudioManager::instance().play(path);
                emit statusMessage(QString("播放：%1").arg(word));
            } else {
                emit statusMessage(QString("音频不存在：%1").arg(path));
            }
  //

    // 确保 audio 目录存在（不存在则自动创建）
   // if (!currentDir.exists("audio")) {
      //  currentDir.mkpath("audio");
   // }

    // 规范地拼接文件路径 (自动处理斜杠)
   // QString path = currentDir.filePath(QString("audio/%1.mp3").arg(word));

    // 判断文件是否存在并播放
    if (QFile::exists(path)) {
        AudioManager::instance().play(path);
        emit statusMessage(QString("播放：%1").arg(word));
        return;
    } else {
        // 建议增加文件不存在时的提示
        emit statusMessage(QString("音频不存在：%1").arg(path));
    }



    QString url = "https://dict.youdao.com/dictvoice?audio=" + word + "&type=2";
    emit statusMessage(QString("下载中：%1").arg(word));
    Downloader::instance().download(url, path);
    connect(&Downloader::instance(), &Downloader::finished,
            this, [this, path](const QString &p){
                if (p == path) {
                    AudioManager::instance().play(p);
                    emit statusMessage("下载完成，开始播放");
                }
            });
}


    void WordBookTab::showEvent(QShowEvent *event)
    {
        QWidget::showEvent(event);

        // ✅ 如果当前年级为空，尝试从 WordManager 获取
        if (currentGrade().isEmpty()) {
            if (manager()) {
                QString grade = manager()->currentGrade();
                if (!grade.isEmpty()) {
                    onGradeChanged(grade);
                }
            }
        } else {
            onGradeChanged(currentGrade());
        }
    }

