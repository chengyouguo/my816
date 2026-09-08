#include "dictationtab.h"
#include "ui_dictationtab.h"
#include "wordmanager.h"
#include "audioplayer.h"
#include <QTimer>
#include <QTableWidgetItem>

DictationTab::DictationTab(QWidget *parent)
    : BaseTab(parent), ui(new Ui::DictationTab)
{
    ui->setupUi(this);

    // 表格初始化
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "错词" << "释义");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setVisible(false);

    // 初始状态
    ui->editInput->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->lblResult->setText("点击「开始测试」");
    ui->lblSummary->clear();

    connect(ui->btnStart,  &QPushButton::clicked, this, &DictationTab::onStart);
    connect(ui->btnPlay,   &QPushButton::clicked, this, &DictationTab::onPlay);
    connect(ui->editInput, &QLineEdit::returnPressed, this, [this]() { onSubmit(); });
}

DictationTab::~DictationTab()
{
    delete ui;
}

/* ================= 开始测试 ================= */

void DictationTab::onStart()
{
    resetUI();
    if (!manager()) return;

    QString grade = currentGrade();
    m_bank = manager()->getSpellList(grade, 10);

    if (m_bank.isEmpty()) {
        ui->lblResult->setText("当前年级无单词");
        return;
    }

    m_total = m_bank.size(); // 实际数量，可能不足10
    m_index = 0;
    m_correct = 0;

    ui->editInput->setEnabled(true);
    ui->btnPlay->setEnabled(true);
    ui->btnStart->setEnabled(false);
    ui->editInput->setFocus();

    emit statusMessage(QString("听写开始，共 %1 题").arg(m_total));
    loadNext();
    emit testStarted();
}

/* ================= 重置 UI ================= */

void DictationTab::resetUI()
{
    m_bank.clear();
    m_current.clear();
    m_index = 0;
    m_correct = 0;
    m_total = 10;

    ui->editInput->clear();
    ui->editInput->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->btnStart->setEnabled(true);

    ui->lblResult->setText("点击「开始测试」");
    ui->lblSummary->clear();

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setVisible(false);
}

/* ================= 加载下一题 ================= */

void DictationTab::loadNext()
{
    if (m_index >= m_total) { finishTest(); return; }

    m_current = m_bank.at(m_index);

    ui->editInput->clear();
    ui->lblTitle->setText(QString("听写测试（%1/%2）").arg(m_index+1).arg(m_total));
    ui->lblResult->setText("请听音拼写，回车提交");
    ui->editInput->setFocus();

    QTimer::singleShot(150, this, &DictationTab::onPlay);
}

/* ================= 播放 ================= */

void DictationTab::onPlay()
{
    QString word = m_current["word"].toString();
    if (word.isEmpty()) return;

    AudioPlayer::instance().play(word);
}

/* ================= 提交/回车 ================= */

void DictationTab::onSubmit()
{
    if (m_current.isEmpty()) return;

    QString input = ui->editInput->text().trimmed().toLower();
    QString answer = m_current["word"].toString().toLower();

    if (input.isEmpty()) {
        ui->lblResult->setText("请输入答案后回车");
        return;
    }

    // ★ 打分：答对/答错
    bool correct = (input == answer);
    manager()->onSpellResult(currentGrade(), m_current["word"].toString(), correct);

    if (correct) {
        ui->lblResult->setText("✅ 正确：" + m_current["word"].toString());
        ++m_correct;
    } else {
        ui->lblResult->setText(QString("❌ 错误，正确答案：%1").arg(m_current["word"].toString()));

        // 记入错词表
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(m_current["word"].toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(m_current["definition"].toString()));
        ui->tableWidget->setVisible(true);
    }

    ++m_index;
    if (m_index >= m_total) {
        QTimer::singleShot(900, this, &DictationTab::finishTest);
    } else {
        QTimer::singleShot(900, this, &DictationTab::loadNext);
    }
}

/* ================= 结束汇总 ================= */

void DictationTab::finishTest()
{
    ui->lblTitle->setText("听写测试结束");
    ui->lblSummary->setText(QString("共 %1 题，正确 %2 题，错误 %3 题")
                                .arg(m_total).arg(m_correct).arg(m_total - m_correct));
    ui->lblSummary->show();
    ui->editInput->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->btnStart->setEnabled(true);

    m_current.clear();

    emit statusMessage(ui->lblSummary->text());
    emit testFinished();
}

/* ================= 年级变化 / 显示 ================= */

void DictationTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty() || !manager()) return;

    // 听写进行中，不允许切换打断
    if (m_index > 0 && m_index < m_total) {
        emit statusMessage(QString("听写进行中，已切换至：%1").arg(grade));
        return;
    }

    resetUI();
    emit statusMessage(QString("听写：%1（%2个）").arg(grade)
                           .arg(manager()->wordsOfGrade(grade).size()));
}

void DictationTab::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // 只在非测试状态下自动刷新
    if (m_index == 0) {
        onGradeChanged(currentGrade());
    }
}