#ifndef WORDBOOKTAB_H
#define WORDBOOKTAB_H

#include "basetab.h"   // ✅ 改这里
#include <QWidget>

namespace Ui { class WordBookTab; }

class WordBookTab : public BaseTab // ✅ 改这里
{
    Q_OBJECT
public:
    explicit WordBookTab(WordManager *mgr, QWidget *parent = nullptr);
    ~WordBookTab() override;

protected:
    void onGradeChanged(const QString &grade) override; // ✅ 必须 override
    void showEvent(QShowEvent *event) override;         // ✅ 加兜底

private slots:
    void on_btnSearch_clicked();
    void on_btnAddWord_clicked();
    void on_btnplayer_clicked();

signals:
    void statusMessage(const QString &msg);

private:
    Ui::WordBookTab *ui;
    QVector<QVariantMap> m_words; // 如果不需要可以删
};

#endif