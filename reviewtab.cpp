#include "reviewtab.h"
#include "ui_reviewtab.h"
#include "wordmanager.h"
#include "audioplayer.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

ReviewTab::ReviewTab(QWidget *parent)
    : BaseTab(parent)
    , ui(new Ui::ReviewTab)
    , m_active(false)
{
    ui->setupUi(this);

    connect(ui->btnStart,    &QPushButton::clicked, this, &ReviewTab::on_btnStart_clicked);
    connect(ui->btnGotIt,    &QPushButton::clicked, this, &ReviewTab::on_btnGotIt_clicked);
    connect(ui->btnDontKnow, &QPushButton::clicked, this, &ReviewTab::on_btnDontKnow_clicked);

    m_revealTimer.setSingleShot(true);
    connect(&m_revealTimer, &QTimer::timeout, this, [this]() {
        if (m_active && m_state == State::Revealing) {
            setState(State::Advancing);
            nextWord();
        }
    });

    ui->lblWord->clear();
    ui->lblPhonetic->clear();
    ui->lblDef->clear();
    setState(State::Idle);
}

ReviewTab::~ReviewTab()
{
    delete ui;
}

/* ================= 状态 ================= */

void ReviewTab::setState(State s)
{
    if (m_state == s) return;
    m_state = s;
    switch (m_state) {
    case State::Idle:
        ui->btnStart->setEnabled(true);
        ui->btnGotIt->setEnabled(false);
        ui->btnDontKnow->setEnabled(false);
        break;
    case State::Showing:
        ui->btnStart->setEnabled(false);
        ui->btnGotIt->setEnabled(true);
        ui->btnDontKnow->setEnabled(true);
        break;
    case State::Revealing:
    case State::Advancing:
        ui->btnStart->setEnabled(false);
        ui->btnGotIt->setEnabled(false);
        ui->btnDontKnow->setEnabled(false);
        break;
    }
}

/* ================= 年级切换 ================= */

void ReviewTab::onGradeChanged(const QString &grade)
{
    Q_UNUSED(grade)
    m_revealTimer.stop();
    m_wordQueue.clear();
    m_queueIndex = 0;
    m_reviewLater.clear();
    m_current.clear();
    ui->lblWord->clear();
    ui->lblPhonetic->clear();
    ui->lblDef->clear();
    setState(State::Idle);
    emit statusMessage(QString("已切换年级：%1").arg(grade));
}

/* ================= 显示/隐藏 ================= */

void ReviewTab::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_active = true;
}

void ReviewTab::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    m_active = false;
    m_revealTimer.stop();
    AudioPlayer::instance().stop();
}

/* ================= 开始 ================= */

void ReviewTab::on_btnStart_clicked()
{
    QString grade = currentGrade();
    m_wordQueue = manager()->getReadList(grade, 10);
    m_queueIndex = 0;
    m_reviewLater.clear();

    if (m_wordQueue.isEmpty()) {
        emit statusMessage("当前年级没有单词！");
        return;
    }

    m_current = m_wordQueue.first();
    showNextWord();
    setState(State::Showing);
}

/* ================= 认识 / 不认识 ================= */

void ReviewTab::on_btnGotIt_clicked()
{
    if (m_state != State::Showing || m_current.isEmpty())
        return;

    QString word = m_current["word"].toString();
    manager()->onReadResult(currentGrade(), word, true); // true = 认识
    ui->lblDef->setText(m_current["definition"].toString());

    setState(State::Advancing);
    nextWord();
}

void ReviewTab::on_btnDontKnow_clicked()
{
    if (m_state != State::Showing || m_current.isEmpty())
        return;

    QString word = m_current["word"].toString();
    manager()->onReadResult(currentGrade(), word, false); // false = 不认识
    ui->lblDef->setText(m_current["definition"].toString());
    m_reviewLater.append(m_current);

    setState(State::Revealing);
    m_revealTimer.start(1500);
}

/* ================= 推进 ================= */

void ReviewTab::nextWord()
{
    m_queueIndex++;
    if (m_queueIndex >= m_wordQueue.size()) {
        if (!m_reviewLater.isEmpty()) {
            m_wordQueue = m_reviewLater;
            m_reviewLater.clear();
            m_queueIndex = 0;
            emit statusMessage(QString("开始复习错词，共 %1 个").arg(m_wordQueue.size()));
        } else {
            ui->lblWord->clear();
            ui->lblPhonetic->clear();
            ui->lblDef->clear();
            setState(State::Idle);
            emit statusMessage("🎉 本年级复习完成！");
            return;
        }
    }

    m_current = m_wordQueue.at(m_queueIndex);
    showNextWord();
}

/* ================= 显示 ================= */

void ReviewTab::showNextWord()
{
    ui->lblWord->setText(m_current["word"].toString());
    ui->lblPhonetic->setText(m_current["phonetic"].toString());
    ui->lblDef->clear();
    playAudio(m_current["word"].toString());
    emit statusMessage(QString("剩余 %1 个").arg(m_wordQueue.size() - m_queueIndex));
    setState(State::Showing);
}

/* ================= 音频 ================= */

void ReviewTab::playAudio(const QString &word)
{
    if (word.isEmpty()) return;
    AudioPlayer::instance().play(word);
}