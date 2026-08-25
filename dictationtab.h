#pragma once
#include "basetab.h"
#include <QWidget>

namespace Ui { class DictationTab; }

class DictationTab : public BaseTab
{
    Q_OBJECT
public:
    explicit DictationTab(WordManager *mgr, QWidget *parent = nullptr);
    ~DictationTab() override;

protected:
    void onGradeChanged(const QString &grade) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::DictationTab *ui = nullptr;
    QVariantMap m_current;

private slots:
    void onPlay();
    void onSubmit();
    void loadNext();

signals:
    void statusMessage(const QString &msg);
};