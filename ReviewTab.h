#pragma once
#include "basetab.h"

namespace Ui {
class ReviewTab;
}

class ReviewTab : public BaseTab
{
    Q_OBJECT
public:
    explicit ReviewTab(WordManager *mgr, QWidget *parent = nullptr);
    ~ReviewTab() override;

protected:
    // ✅ 唯一入口：年级变化
    void onGradeChanged(const QString &grade) override;

    // ✅ 只补一次当前状态
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
signals:
    // 👇 就是这一行，补上它！
    void statusMessage(const QString &msg);
private:
    // ✅ UI + 当前数据
    Ui::ReviewTab *ui = nullptr;
    QVector<QVariantMap> m_words;
    QVariantMap m_currentWord;

    // ✅ 业务私有
    void loadNextWord();
    void playAudio(const QString &word);

private slots:
    void on_btnGotIt_clicked();
    void on_btnDontKnow_clicked();
    // reviewtab.h
//private:
   // bool m_silentOnGradeChange = true; // ✅ 默认：切年级静音
};