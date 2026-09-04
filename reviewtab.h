#ifndef REVIEWTAB_H
#define REVIEWTAB_H

#include "WeightedWordPool.h"
#include "basetab.h"
#include <QWidget>
#include <QQueue>
#include <QVariantMap>
#include <QTimer>

namespace Ui { class ReviewTab; }
class WordManager;

class ReviewTab : public BaseTab
{
    Q_OBJECT

public:
    explicit ReviewTab(WordManager *mgr, QWidget *parent = nullptr);
    ~ReviewTab();

    void onGradeChanged(const QString &grade) override;

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void on_btnStart_clicked();
    void on_btnGotIt_clicked();
    void on_btnDontKnow_clicked();

private:
    // ★ 状态枚举
    enum class State {
        Idle,        // 初始/完成，等待按开始
        Showing,     // 显示单词，可操作
        Revealing,   // 已揭示释义，等待自动下一步
        Advancing    // 正在加载下一个（防重入）

    };
   QList<QVariantMap> m_words;
    void setState(State s);          // ★ 唯一的状态切换入口
    void loadNextWord();
    void playAudio(const QString &word);
    void refreshButtons();           // ★ 根据状态更新按钮可用状态

    Ui::ReviewTab *ui;

    WeightedWordPool m_pool;                // 换成你的实际类型
    QQueue<QVariantMap> m_reviewLater;
    QVariantMap m_currentWord;

    State m_state = State::Idle;
    bool m_active = false;
    bool m_resumeMode = false;

    QTimer m_revealTimer;           // 揭示释义后的等待定时器
signals:                   // <-- 加上这段
    void statusMessage(const QString &msg);
};

#endif // REVIEWTAB_H