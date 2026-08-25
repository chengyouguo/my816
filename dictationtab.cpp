#include "dictationtab.h"
#include "ui_dictationtab.h"
#include "wordmanager.h"
#include "audiomanager.h"
#include <QTimer>
#include <QTableWidgetItem>
#include <QDir>

DictationTab::DictationTab(WordManager *mgr, QWidget *parent)
    : BaseTab(mgr, parent), ui(new Ui::DictationTab)
{
    ui->setupUi(this);

    // ✅ 表格初始化
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "错词" << "释义");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(ui->btnPlay,   &QPushButton::clicked, this, &DictationTab::onPlay);
    connect(ui->btnSubmit, &QPushButton::clicked, this, &DictationTab::onSubmit);
}

DictationTab::~DictationTab()
{
    delete ui;
}

/* ================= 年级变化（唯一数据源） ================= */

void DictationTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty() || !manager())
        return;

    auto list = manager()->wordsOfGrade(grade);
    if (list.isEmpty()) {
        ui->lblResult->setText("当前年级无单词");
        emit statusMessage(QString("听写：%1 无单词").arg(grade));
        m_current.clear();
        return;
    }

    m_current = list.first(); // ✅ 简单策略：取第一个
    ui->editInput->clear();
    ui->lblResult->setText("点击播放");

    emit statusMessage(QString("听写：%1（%2个）").arg(grade).arg(list.size()));
}

/* ================= 生命周期（兜底） ================= */

void DictationTab::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // ✅ 二级兜底（和 ReviewTab 一模一样）
    QString grade = currentGrade();
    if (grade.isEmpty() && manager()) {
        grade = manager()->currentGrade();
    }

    if (!grade.isEmpty()) {
        onGradeChanged(grade);
    }
}

/* ================= 业务 ================= */

void DictationTab::loadNext()
{
    if (!manager()) return;

    auto list = manager()->wordsOfGrade(currentGrade());
    if (list.isEmpty()) return;

    m_current = list.first();
    ui->editInput->clear();
    ui->lblResult->setText("点击播放");
}

void DictationTab::onPlay()
{
    if (m_current.isEmpty()) return;

    QString word = m_current["word"].toString();
    QString path = QDir::currentPath() + "/audio/" + word + ".mp3";

    if (QFile::exists(path)) {
        AudioManager::instance().play(path);
        emit statusMessage(QString("播放：%1").arg(word));
    } else {
        emit statusMessage(QString("音频不存在：%1").arg(word));
        // TODO: 下载逻辑
    }
}

void DictationTab::onSubmit()
{
    if (m_current.isEmpty()) return;

    QString input = ui->editInput->text().trimmed().toLower();
    QString answer = m_current["word"].toString().toLower();

    if (input.isEmpty()) {
        ui->lblResult->setText("请输入答案");
        return;
    }

    if (input == answer) {
        ui->lblResult->setText("✅ 正确");
        emit statusMessage("听写正确");
    } else {
        ui->lblResult->setText("❌ 错误：" + m_current["word"].toString());
        emit statusMessage("听写错误：" + m_current["word"].toString());

        // ✅ 错词进表
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0,
            new QTableWidgetItem(m_current["word"].toString()));
        ui->tableWidget->setItem(row, 1,
            new QTableWidgetItem(m_current["definition"].toString()));
    }

    QTimer::singleShot(1200, this, &DictationTab::loadNext);
}