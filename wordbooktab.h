#ifndef WORDBOOKTAB_H
#define WORDBOOKTAB_H

#include "basetab.h"   // 提供 manager() / refresh() / onGradeChanged()
#include <QWidget>

namespace Ui {
class WordBookTab;
}

class WordBookTab : public BaseTab
{
    Q_OBJECT
public:
    explicit WordBookTab(WordManager *mgr, QWidget *parent = nullptr);
    ~WordBookTab();

signals:
    void statusMessage(const QString &msg);

private slots:
    void onGradeChanged(const QString &grade);

    void on_btnSearch_clicked();
    void on_btnAddWord_clicked();
    void on_btnplayer_clicked();   // 播放 / 下载后播放

private:
    Ui::WordBookTab *ui;

    /// 播放指定单词：有缓存直接播，无缓存走下载（单次连接，不累积）
    void playWord(const QString &word);
    void showEvent(QShowEvent *event);
};

#endif // WORDBOOKTAB_H
