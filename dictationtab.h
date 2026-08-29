#pragma once
#include "basetab.h"
#include <QMap>
#include <QString>

// 前置声明，告诉编译器 Ui::DictationTab 是个类，具体定义在别处
namespace Ui {
class DictationTab;
}

class DictationTab : public BaseTab {
    Q_OBJECT
public:
    explicit DictationTab(WordManager *mgr, QWidget *parent = nullptr);
    ~DictationTab() override;

    void onGradeChanged(const QString &grade) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onStart();
    void onPlay();
    void onSubmit();

private:
    void loadNext();
    void finishTest();
    void resetUI();

    Ui::DictationTab *ui;          // ← 这里只放指针，没问题
    QList<QMap<QString, QVariant>> m_bank;
    QMap<QString, QVariant>        m_current;
    int  m_index = 0;
    int  m_total = 10;
    int  m_correct = 0;
signals:                   // <-- 加上这段
    void statusMessage(const QString &msg);
};