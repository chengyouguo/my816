#include "mainwindow.h"
#include "wordbooktab.h"
#include "reviewtab.h"
#include "dictationtab.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMenuBar>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ─────────────────────────────
    // 1. 初始化数据库
    // ─────────────────────────────
    m_mgr = new WordManager(this);
    if (!m_mgr->init("words.db")) {
        statusBar()->showMessage("数据库初始化失败", 5000);
        return;
    }
//
    m_mgr->initCurrentGrade();

  //  qDebug() << "DEBUG startup grade:" << m_mgr->currentGrade();
    // ─────────────────────────────
    // 3. 填充下拉框（关键：blockSignals）
    ui->comboGrade->blockSignals(true);
    ui->comboGrade->addItems(m_mgr->grades());
    ui->comboGrade->blockSignals(false);

    QSettings settings;
    QString lastGrade = settings.value("lastGrade", "").toString();

    if (!lastGrade.isEmpty() && m_mgr->gradeExists(lastGrade)) {
        m_mgr->setCurrentGrade(lastGrade);
        ui->comboGrade->setCurrentText(lastGrade);   // ✅ 关键：UI 选中
        qDebug() << "DEBUG [Constructor] Using saved grade:" << lastGrade;
    } else {
        QStringList grades = m_mgr->grades();
        if (!grades.isEmpty()) {
            m_mgr->setCurrentGrade(grades.first());
            ui->comboGrade->setCurrentText(grades.first()); // ✅ 关键：UI 选中
            qDebug() << "DEBUG [Constructor] No saved grade, using first grade:" << grades.first();
        }
    }

     //对齐 UI 与 Manager
    const QString cur = m_mgr->currentGrade();
    const int idx = ui->comboGrade->findText(cur);
    if (idx >= 0)
        ui->comboGrade->setCurrentIndex(idx);

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
    // 5. 三个业务 Tab（只读当前年级）
    // ─────────────────────────────
    wordBook     = new WordBookTab(m_mgr, this);
    reviewTab    = new ReviewTab(m_mgr, this);
    dictationTab = new DictationTab(m_mgr, this);

    ui->tabWidget->addTab(wordBook,    "单词本");
    ui->tabWidget->addTab(reviewTab,   "听看");
    ui->tabWidget->addTab(dictationTab,"听写");

    // ─────────────────────────────
    // 6. 年级管理按钮
    // ─────────────────────────────
    connect(ui->btnAddGrade, &QPushButton::clicked,
            this, &MainWindow::on_btnAddGrade_clicked);
    connect(ui->btnDelGrade, &QPushButton::clicked,
            this, &MainWindow::on_btnDelGrade_clicked);

    // ─────────────────────────────
    // 7. 下拉框变化 → 唯一修改入口
    connect(ui->btnApplyGrade, &QPushButton::clicked, this, [this](){
        if (m_pendingGrade.isEmpty())
            return;

        // 真正生效
        m_mgr->setCurrentGrade(m_pendingGrade);
        QSettings().setValue("lastGrade", m_pendingGrade);

        qDebug() << "DEBUG [Apply] Grade committed:" << m_pendingGrade;
        statusBar()->showMessage("年级已切换：" + m_pendingGrade);
    });
    // ─────────────────────────────
    connect(ui->comboGrade, &QComboBox::currentTextChanged,
            this, [&](const QString &grade){
                if (!grade.isEmpty())
                    m_pendingGrade = grade;

              //  QSettings().setValue("lastGrade", grade);
                // 【调试】每次切换时，立即打印存了什么
                // ─────────────────────────────
                qDebug() << "DEBUG [Switch] Saved grade to QSettings:" << grade;
            });

    // ─────────────────────────────
    // 8. Tab 状态消息统一接管
    // ─────────────────────────────
    auto handle = [this, permanent](const QString &msg){
        if (msg.startsWith("当前年级"))
            permanent->setText(msg);
        else
           // statusBar()->showMessage(msg, 3000);
        statusBar()->showMessage(msg);
    };
    connect(wordBook,     &WordBookTab::statusMessage, this, handle);
    connect(reviewTab,    &ReviewTab::statusMessage,    this, handle);
    connect(dictationTab, &DictationTab::statusMessage, this, handle);

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

    if (m_mgr->addGrade(g)) {
        ui->comboGrade->addItem(g);
        ui->editNewGrade->clear();
        statusBar()->showMessage("已添加年级：" + g);
    } else {
        statusBar()->showMessage("年级已存在或添加失败");
    }
}

// ─────────────────────────────
// 删除年级（含“新当前年级保存”）
// ─────────────────────────────
void MainWindow::on_btnDelGrade_clicked()
{
    const int index = ui->comboGrade->currentIndex();
    if (index < 0)
        return;

    const QString g = ui->comboGrade->itemText(index);
    if (ui->comboGrade->count() <= 1) {
        statusBar()->showMessage("至少保留一个年级");
        return;
    }

    if (m_mgr->removeGrade(g)) {
        ui->comboGrade->removeItem(index);

        // 如果删掉的是当前年级 → 自动切 + 保存
        if (m_mgr->currentGrade() == g) {
            const QString replacement = ui->comboGrade->currentText();
            ui->comboGrade->setCurrentIndex(ui->comboGrade->currentIndex());
            m_mgr->setCurrentGrade(replacement);
            QSettings().setValue("lastGrade", replacement);
        }

        statusBar()->showMessage("已删除年级：" + g);
    }
}