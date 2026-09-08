#ifndef DICTATIONTAB_H
#define DICTATIONTAB_H

#include "basetab.h"
#include <QVariantMap>
#include <QVector>

namespace Ui { class DictationTab; }

class DictationTab : public BaseTab
{
    Q_OBJECT
public:
    explicit DictationTab(QWidget *parent = nullptr);
    ~DictationTab();

    void onGradeChanged(const QString &grade) override;

signals:
    void testStarted();
    void testFinished();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onStart();
    void onPlay();
    void onSubmit();

private:
    void resetUI();
    void loadNext();
    void finishTest();

    Ui::DictationTab *ui;
    QVector<QVariantMap> m_bank;
    QVariantMap m_current;
    int m_index = 0;
    int m_correct = 0;
    int m_total = 10;
};

#endif // DICTATIONTAB_H