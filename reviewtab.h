#ifndef REVIEWTAB_H
#define REVIEWTAB_H

#include "basetab.h"
#include <QVariantMap>
#include <QVector>
#include <QTimer>

namespace Ui { class ReviewTab; }

class ReviewTab : public BaseTab
{
    Q_OBJECT
public:
    explicit ReviewTab(QWidget *parent = nullptr);
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
    enum class State { Idle, Showing, Revealing, Advancing };

    void setState(State s);
    void showNextWord();
    void nextWord();
    void playAudio(const QString &word);

    Ui::ReviewTab *ui;
    bool m_active = false;
    State m_state = State::Idle;

    QVector<QVariantMap> m_wordQueue;
    int m_queueIndex = 0;
    QVector<QVariantMap> m_reviewLater;
    QVariantMap m_current;

    QTimer m_revealTimer;
};

#endif // REVIEWTAB_H