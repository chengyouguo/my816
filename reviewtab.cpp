#include "reviewtab.h"
#include "ui_reviewtab.h"
#include "wordmanager.h"
#include "audiomanager.h"
#include "downloader.h"

#include <QDir>
#include <QFile>
#include <QTimer>

ReviewTab::ReviewTab(WordManager *mgr, QWidget *parent)
    : BaseTab(mgr, parent), ui(new Ui::ReviewTab)
   // ：manager()->currentGrade()
{
    ui->setupUi(this);

    connect(ui->btnGotIt, &QPushButton::clicked,
            this, &ReviewTab::on_btnGotIt_clicked);
    connect(ui->btnDontKnow, &QPushButton::clicked,
            this, &ReviewTab::on_btnDontKnow_clicked);
    QString grade = manager()->currentGrade();
    //QString grade = currentGrade(); // ✅ 用基类接口
    if (grade.isEmpty()) {
        emit statusMessage("请先在主窗口选择年级");
        return;
    }
   // onGradeChanged(currentGrade());

}

ReviewTab::~ReviewTab()
{
    delete ui;
}

/* ================= 年级变化（唯一数据源） ================= */

void ReviewTab::onGradeChanged(const QString &grade)
{
    if (grade.isEmpty() || !manager())
        return;
     //m_currentGrade = grade;  // ← 补这行，调父类变量
m_currentWord.clear();  // 切年级时清掉当前词
  //  emit statusMessage(QString("听看模式：切换到 %1").arg(grade));
// emit statusMessage(QString("听看模式：%1（%2个）").arg(grade).arg(m_currentWord.size()));
emit statusMessage(QString("听看：%1（%2个）").arg(grade)
                       .arg(manager()->wordsOfGrade(grade).size()));
    m_words = manager()->getWeightedWordsOfGrade(grade);
    loadNextWord();
}

/* ================= 生命周期 ================= */

void ReviewTab::showEvent(QShowEvent *event)
{
    //
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

void ReviewTab::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    AudioManager::instance().stop();
}

/* ================= 业务私有 ================= */

void ReviewTab::loadNextWord()
{
    if (m_words.isEmpty()) {
        ui->lblWord->clear();
        ui->lblPhonetic->clear();
        ui->lblDef->clear();
        return;
    }

    m_currentWord = m_words.takeFirst();
    const QString word = m_currentWord["word"].toString();

    ui->lblWord->setText(word);
    ui->lblPhonetic->setText(m_currentWord["phonetic"].toString());
    ui->lblDef->clear();

    playAudio(word);
}

void ReviewTab::playAudio(const QString &word)
{
    const QString path = QDir::currentPath() + "/audio/" + word + ".mp3";
    if (QFile::exists(path)) {
        AudioManager::instance().play(path);
        return;
    }

    QDir().mkpath(QFileInfo(path).absolutePath());
    const QString url = "https://dict.youdao.com/dictvoice?audio=" + word + "&type=2";

    Downloader::instance().download(url, path);
    connect(&Downloader::instance(), &Downloader::finished,
            this, [this, path](const QString &p){
                if (p == path)
                    AudioManager::instance().play(path);
            }, Qt::UniqueConnection);
}

/* ================= 用户操作 ================= */

void ReviewTab::on_btnGotIt_clicked()
{if (m_currentWord.isEmpty()) return;  // ← 加这个
    // 调用 markWordUnknown 的那一行附近加：
    qDebug() << "About to mark unknown, grade from:" << currentGrade() << "word:" << m_currentWord;
    manager()->markWordKnown(manager()->currentGrade(), m_currentWord["word"].toString());
    emit statusMessage(QString("已掌握：%1").arg(m_currentWord["word"].toString()));
    loadNextWord();
}

void ReviewTab::on_btnDontKnow_clicked()
{if (m_currentWord.isEmpty()) return;  // ← 加这个
    ui->lblDef->setText(m_currentWord["definition"].toString());

    manager()->markWordUnknown(manager()->currentGrade(), m_currentWord["word"].toString());
    emit statusMessage(QString("待巩固：%1").arg(m_currentWord["word"].toString()));
    QTimer::singleShot(1500, this, &ReviewTab::loadNextWord);
}