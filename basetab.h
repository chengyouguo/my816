#ifndef BASETAB_H
#define BASETAB_H

#include <QWidget>
#include <QString>
#include "wordmanager.h"

class BaseTab : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTab(QWidget *parent = nullptr);
    virtual ~BaseTab() = default;

    // 子类通过这两个访问总管
    WordManager *manager() const { return WordManager::instance(); }
    QString currentGrade() const { return m_currentGrade; }

signals:
    void statusMessage(const QString &msg);

public slots:
    virtual void onGradeChanged(const QString &grade) = 0;

protected slots:
    void handleGradeChanged(const QString &grade);

protected:
    void setGrade(const QString &grade);
    void ensureGradeAndShow();
    void refresh();

    QString m_currentGrade;
};

#endif // BASETAB_H