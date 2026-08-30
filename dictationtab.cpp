#include "dictationtab.h"
#include "ui_dictationtab.h"
#include "wordmanager.h"
#include "audiomanager.h"
#include <QTimer>
#include <QDir>
#include <QTableWidgetItem>
#include <QRandomGenerator>

DictationTab::DictationTab(WordManager *mgr, QWidget *parent)
    : BaseTab(mgr, parent), ui(new Ui::DictationTab), m_index(0), m_correct(0)
{
    ui->setupUi(this);

    // 表格初始化
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "错词" << "释义");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setVisible(false);

    // 初始状态：未开始，输入框禁用
    ui->editInput->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->lblResult->setText("点击「开始测试」");
    ui->lblSummary->clear();

    connect(ui->btnStart,  &QPushButton::clicked, this, &DictationTab::onStart);
    connect(ui->btnPlay,   &QPushButton::clicked, this, &DictationTab::onPlay);
    connect(ui->editInput, &QLineEdit::returnPressed, this, &DictationTab::onSubmit);
}

DictationTab::~DictationTab() { delete ui; }

/* ================= 开始测试 ================= */

void DictationTab::onStart()
{
    resetUI();   // ★ 第一句就复位（清计数、清表格、清汇总、禁输入框）
    if (!manager()) return;

    QString grade = currentGrade().isEmpty() ? manager()->currentGrade() : currentGrade();
    auto pool = manager()->wordsOfGrade(grade);
    if (pool.isEmpty()) {
        ui->lblResult->setText("当前年级无单词");
        return;
    }

    // 随机抽取最多 10 个（不重复）
    m_total = qMin(10, pool.size());
    QList<int> idxs;
    for (int i = 0; i < pool.size(); ++i) idxs << i;
    std::shuffle(idxs.begin(), idxs.end(), *QRandomGenerator::global());
    m_bank.clear();
    for (int i = 0; i < m_total; ++i) m_bank << pool.at(idxs.at(i));

    m_index = 0;
    m_correct = 0;
    // 启用测试控件
    ui->editInput->setEnabled(true);
    ui->btnPlay->setEnabled(true);
    ui->btnStart->setEnabled(false);
    ui->editInput->setFocus();
    emit statusMessage(QString("听写开始，共 %1 题").arg(m_total));
    loadNext();
}
void DictationTab::resetUI()
{
    m_bank.clear();
    m_current.clear();
    m_index = 0;
    m_correct = 0;
    m_total = 10;          // ★ 复位默认总数，防旧值

    ui->editInput->clear();
    ui->editInput->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->btnStart->setEnabled(true);

    ui->lblResult->setText("点击「开始测试」");
    ui->lblSummary->clear();          // ★ 清掉“共7题，正确3题...”
   // ui->lblTitle->setText("听写测试");

    ui->tableWidget->setRowCount(0);  // ★ 清错词表
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
    if (m_current.isEmpty()) return;
    QString word = m_current["word"].toString();
    QString path = QDir::currentPath() + "/audio/" + word + ".mp3";
    if (QFile::exists(path)) {
        AudioManager::instance().play(path);
       // emit statusMessage(QString("播放：%1").arg(word));
    } else {
        emit statusMessage(QString("音频不存在：%1，已跳过").arg(word));
        // 缺音频：自动跳过本题，记为错误
        m_current["__skipped__"] = true;
        onSubmit(); // 走提交流程，内部会判跳过
    }
}

/* ================= 提交/回车 ================= */

void DictationTab::onSubmit()
{
    if (m_current.isEmpty()) return;

    QString input = ui->editInput->text().trimmed().toLower();
    QString answer = m_current["word"].toString().toLower();
    bool skipped = m_current.value("__skipped__", false).toBool();

    if (!skipped && input.isEmpty()) {
        ui->lblResult->setText("请输入答案后回车");
        return;
    }

    if (skipped) {
        ui->lblResult->setText("⚠ 音频缺失，已跳过：" + m_current["word"].toString());
    } else if (input == answer) {
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
    // 汇总只在结束显示
   ui->lblSummary->setText(QString("共 %1 题，正确 %2 题，错误 %3 题")
                               .arg(m_total).arg(m_correct).arg(m_total - m_correct));
    ui->lblSummary->hide();
    ui->editInput->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->btnStart->setEnabled(true);
    m_current.clear();
    emit statusMessage(ui->lblSummary->text());
}

/* ================= 年级变化 / 显示 ================= */

void DictationTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty() || !manager()) return;
    resetUI();
    emit statusMessage(QString("听写：%1（%2个）").arg(grade)
                       .arg(manager()->wordsOfGrade(grade).size()));
}



void DictationTab::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    QString grade = currentGrade();
    if (grade.isEmpty() && manager()) grade = manager()->currentGrade();
    if (!grade.isEmpty()) onGradeChanged(grade);
}