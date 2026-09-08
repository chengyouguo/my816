#include "mainwindow.h"
#include "wordbooktab.h"
#include "reviewtab.h"
#include "dictationtab.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMenuBar>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ─────────────────────────────
    // 1. 初始化单例数据库
    // ─────────────────────────────
   //if (!WordManager::instance()->init("words.db")) {
       // statusBar()->showMessage("数据库初始化失败", 5000);
   //  return;
 //}

    // ─────────────────────────────
    // 2. 填充下拉框
    // ─────────────────────────────
    ui->comboGrade->blockSignals(true);
    ui->comboGrade->addItems(WordManager::instance()->grades());
    ui->comboGrade->blockSignals(false);

    // ─────────────────────────────
    // 3. 恢复上次年级
    // ─────────────────────────────
    QSettings settings;
    QString lastGrade = settings.value("lastGrade", "").toString();
    QStringList grades = WordManager::instance()->grades();

    if (!lastGrade.isEmpty() && grades.contains(lastGrade)) {
        WordManager::instance()->setCurrentGrade(lastGrade);
        ui->comboGrade->setCurrentText(lastGrade);
    } else if (!grades.isEmpty()) {
        WordManager::instance()->setCurrentGrade(grades.first());
        QSettings().setValue("lastGrade", grades.first()); // ← 就加这行
        ui->comboGrade->setCurrentText(grades.first());
    }

    // ─────────────────────────────
    // 4. 窗口基础属性
    // ─────────────────────────────
    setWindowTitle("GradeWordApp");
    resize(900, 650);
    setMenuBar(new QMenuBar(this));

    QLabel *permanent = new QLabel(this);
    permanent->setStyleSheet("padding-left:10px;color:#666;");
    statusBar()->addPermanentWidget(permanent);

    // ─────────────────────────────
    // 5. 三个业务 Tab（只传 parent）
    // ─────────────────────────────
    wordBook     = new WordBookTab(this);
    reviewTab    = new ReviewTab(this);
    dictationTab = new DictationTab(this);

    ui->tabWidget->addTab(wordBook,    "单词本");
    ui->tabWidget->addTab(reviewTab,   "听看");
    ui->tabWidget->addTab(dictationTab,"听写");
    // 强制触发一次年级变化信号，让所有页面刷新
    emit WordManager::instance()->gradeChanged(WordManager::instance()->currentGrade());
    // ─────────────────────────────
    // 6. 年级管理按钮
    // ─────────────────────────────
    connect(ui->btnAddGrade, &QPushButton::clicked,
            this, &MainWindow::on_btnAddGrade_clicked);
    connect(ui->btnDelGrade, &QPushButton::clicked,
            this, &MainWindow::on_btnDelGrade_clicked);

    // ─────────────────────────────
    // 7. 下拉框变化 → 直接生效
    // ─────────────────────────────
    connect(ui->comboGrade, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index){
                if (index < 0) return;
                QString g = ui->comboGrade->itemText(index);
                WordManager::instance()->setCurrentGrade(g);
                QSettings().setValue("lastGrade", g);
                statusBar()->showMessage("年级已切换：" + g);
            });

    // ─────────────────────────────
    // 8. Tab 状态消息统一接管
    // ─────────────────────────────
    auto handle = [this, permanent](const QString &msg){
        if (msg.startsWith("当前年级"))
            permanent->setText(msg);
        else
            statusBar()->showMessage(msg);
    };
    connect(wordBook,     &WordBookTab::statusMessage,     this, handle);
    connect(reviewTab,    &ReviewTab::statusMessage,       this, handle);
    connect(dictationTab, &DictationTab::statusMessage,    this, handle);

    // 听写测试锁定 Tab
    connect(dictationTab, &DictationTab::testStarted,
            this, &MainWindow::onTestStarted);
    connect(dictationTab, &DictationTab::testFinished,
            this, &MainWindow::onTestFinished);

    // ─────────────────────────────
    // 9. 启动页
    // ─────────────────────────────
    ui->tabWidget->setCurrentIndex(1);
    statusBar()->showMessage("程序已启动，数据库已连接");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ─────────────────────────────
// 添加年级
// ─────────────────────────────
void MainWindow::on_btnAddGrade_clicked()
{
    const QString g = ui->editNewGrade->text().trimmed();
    if (g.isEmpty())
        return;

    if (WordManager::instance()->addGrade(g)) {
        ui->comboGrade->addItem(g);
        ui->editNewGrade->clear();
        statusBar()->showMessage("已添加年级：" + g);
    } else {
        statusBar()->showMessage("年级已存在或添加失败");
    }
}

// ─────────────────────────────
// 删除年级
// ─────────────────────────────
void MainWindow::on_btnDelGrade_clicked()
{
    int index = ui->comboGrade->currentIndex();
    if (index < 0)
        return;

    QString g = ui->comboGrade->itemText(index);
    if (ui->comboGrade->count() <= 1) {
        statusBar()->showMessage("至少保留一个年级");
        return;
    }

    if (WordManager::instance()->removeGrade(g)) {
        ui->comboGrade->removeItem(index);

        // 删掉的是当前年级 → 自动切到新选中项
        if (WordManager::instance()->currentGrade() == g) {
            QString replacement = ui->comboGrade->currentText();
            WordManager::instance()->setCurrentGrade(replacement);
            QSettings().setValue("lastGrade", replacement);
        }

        statusBar()->showMessage("已删除年级：" + g);
    }
}

// ─────────────────────────────
// 听写锁定
// ─────────────────────────────
void MainWindow::onTestStarted()
{
    m_testInProgress = true;

    int count = ui->tabWidget->count();
    for (int i = 0; i < count; ++i) {
        if (ui->tabWidget->widget(i) != dictationTab)
            ui->tabWidget->setTabEnabled(i, false);
    }

    ui->comboGrade->setEnabled(false);
    ui->btnAddGrade->setEnabled(false);
    ui->btnDelGrade->setEnabled(false);
}

void MainWindow::onTestFinished()
{
    m_testInProgress = false;

    int count = ui->tabWidget->count();
    for (int i = 0; i < count; ++i)
        ui->tabWidget->setTabEnabled(i, true);

    ui->comboGrade->setEnabled(true);
    ui->btnAddGrade->setEnabled(true);
    ui->btnDelGrade->setEnabled(true);
}