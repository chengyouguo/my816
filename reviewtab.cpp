#include "reviewtab.h"
#include "ui_reviewtab.h"
#include "wordmanager.h"
#include "audiomanager.h"
#include "audioplayer.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QScopeGuard>

ReviewTab::ReviewTab(WordManager *mgr, QWidget *parent)
    : BaseTab(mgr, parent)
    , ui(new Ui::ReviewTab)
    , m_active(false)
    , m_resumeMode(false)
{
    ui->setupUi(this);

    connect(ui->btnStart,    &QPushButton::clicked, this, &ReviewTab::on_btnStart_clicked);
    connect(ui->btnGotIt,    &QPushButton::clicked, this, &ReviewTab::on_btnGotIt_clicked);
    connect(ui->btnDontKnow, &QPushButton::clicked, this, &ReviewTab::on_btnDontKnow_clicked);

    // ★ 定时器只配置一次，永不重复 connect
    m_revealTimer.setSingleShot(true);
    connect(&m_revealTimer, &QTimer::timeout, this, [this]() {
        if (m_active && m_state == State::Revealing) {
            setState(State::Advancing);
            loadNextWord();
        }
    });

    ui->lblWord->clear();
    ui->lblPhonetic->clear();
    ui->lblDef->clear();
    setState(State::Idle);

    QString grade = manager()->currentGrade();
    if (grade.isEmpty())
        emit statusMessage("请先在主窗口选择年级");
    else
        emit statusMessage(QString("听看待命：%1").arg(grade));
}

ReviewTab::~ReviewTab()
{
    delete ui;
}

/* ================= 状态管理（核心） ================= */

void ReviewTab::setState(State s)
{
    if (m_state == s)
        return;
    m_state = s;
    refreshButtons();
}

void ReviewTab::refreshButtons()
{
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

/* ================= 年级变化 ================= */

void ReviewTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty() || !manager())
        return;

    m_revealTimer.stop();
    m_pool.reset(QVector<QVariantMap>());
    m_reviewLater.clear();
    m_currentWord.clear();
    ui->lblWord->clear();
    ui->lblPhonetic->clear();
    ui->lblDef->clear();
    setState(State::Idle);

    emit statusMessage(QString("已切换年级：%1，按开始键加载").arg(grade));
}

/* ================= 生命周期 ================= */

void ReviewTab::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_active = true;

    QString grade = manager()->currentGrade();
    if (grade.isEmpty())
        return;

    if (!m_resumeMode && m_state == State::Idle && m_pool.isEmpty()) {
        m_pool.reset(manager()->getWeightedWordsOfGrade(grade));
        m_reviewLater.clear();
        m_currentWord.clear();
        ui->lblWord->clear();
        ui->lblPhonetic->clear();
        ui->lblDef->clear();
    }
    emit statusMessage(QString("听看模式：%1（剩余 %2 个）")
                           .arg(grade).arg(m_pool.remaining()));
}

void ReviewTab::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    m_active = false;
    m_revealTimer.stop();
    AudioPlayer::instance().stop();
}

/* ================= 按钮事件 ================= */


void ReviewTab::on_btnStart_clicked()
{
    if (!manager() || !ui)
        return;

    m_revealTimer.stop();              // ← 用 m_revealTimer，不是 m_autoNextTimer
    m_state = State::Idle;             // ← 重置状态，不需要 m_loadingNext

    QString grade = manager()->currentGrade();
    if (grade.isEmpty()) {
        emit statusMessage("请先在主窗口选择年级");
        return;
    }

    // ★ 用你实际有的函数名
    m_pool.reset(manager()->getWeightedWordsOfGrade(grade));
    m_reviewLater.clear();
    m_currentWord.clear();

    if (m_pool.isEmpty()) {
        ui->lblWord->clear();
        ui->lblPhonetic->clear();
        ui->lblDef->clear();
        setState(State::Idle);
        emit statusMessage("本年级暂无可复习单词");
        return;
    }

    setState(State::Advancing);
    loadNextWord();
}

void ReviewTab::on_btnDontKnow_clicked()
{
    if (m_state != State::Showing)
        return;

    const QString word = m_currentWord["word"].toString();
    const QString grade = manager()->currentGrade();  // ★ 这行可能崩

    ui->lblDef->setText(m_currentWord["definition"].toString());
    manager()->markWordUnknown(grade, word);
    ui->lblDef->setText(m_currentWord["definition"].toString());
    manager()->markWordUnknown(manager()->currentGrade(), m_currentWord["word"].toString());
    emit statusMessage(QString("待巩固：%1（剩余 %2 个）")
                           .arg(m_currentWord["word"].toString())
                           .arg(m_pool.remaining()));

    m_reviewLater.enqueue(m_currentWord);

    // ★ 进入 Revealing，等待定时器自动下一步
    setState(State::Revealing);
    m_revealTimer.start(1500);
}

/* ================= 业务私有 ================= */

void ReviewTab::loadNextWord()
{
    if (m_state != State::Advancing)
        return;
    Q_ASSERT(manager() != nullptr);
    Q_ASSERT(ui != nullptr);
    if (!manager() || !ui) {
        qDebug() << "manager() or ui is null! state=" << (int)m_state;
        return;
    }
    auto guard = qScopeGuard([this]() {
        if (m_state == State::Advancing)
            setState(State::Idle);
    });

    if (m_pool.isEmpty()) {
        if (!m_reviewLater.isEmpty()) {
            QVector<QVariantMap> reviewList;
            while (!m_reviewLater.isEmpty())
                reviewList.append(m_reviewLater.dequeue());
            m_pool.reset(reviewList);
            emit statusMessage(QString("开始复习错词，共 %1 个").arg(m_pool.remaining()));
        } else {
            ui->lblWord->clear();
            ui->lblPhonetic->clear();
            ui->lblDef->clear();
            emit statusMessage("本年级复习完成！");
            return;
        }
    }

    QVariantMap w = m_pool.draw();
    if (w.isEmpty()) {
        emit statusMessage("本年级复习完成！");
        return;
    }

    m_currentWord = w;
    ui->lblWord->setText(w["word"].toString());
    ui->lblPhonetic->setText(w["phonetic"].toString());
    ui->lblDef->clear();
    emit statusMessage(QString("剩余 %1 个").arg(m_pool.remaining()));
    playAudio(w["word"].toString());

    setState(State::Showing);
}

void ReviewTab::playAudio(const QString &word)
{
  //  Q_UNUSED(word);
    // 只播本地有效文件，不下载



  QString path = QDir::currentPath() + "/audio/" + word + ".mp3";
  if (QFile::exists(path)) {
      //  AudioManager::instance().play(path);
      AudioPlayer::instance().play(word);
      // emit statusMessage(QString("播放：%1").arg(word));
  } else {
      emit statusMessage(QString("音频不存在：%1，已跳过").arg(word));


  }
}

void ReviewTab::on_btnGotIt_clicked()
{
    if (m_state != State::Showing) return;
    // 标记已知（错词数-1）
    manager()->markWordKnown(manager()->currentGrade(), m_currentWord["word"].toString());
    ui->lblDef->setText(m_currentWord["definition"].toString());
    setState(State::Advancing);
    loadNextWord();
}
