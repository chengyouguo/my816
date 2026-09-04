#include "basetab.h"
#include <QDebug>

BaseTab::BaseTab(WordManager *mgr, QWidget *parent)
    : QWidget(parent), m_mgr(mgr)
{
    Q_ASSERT(mgr);

    connect(m_mgr, &WordManager::gradeChanged,
            this, &BaseTab::handleGradeChanged);
}

void BaseTab::refresh()
{
    onRefresh();
}

void BaseTab::onRefresh()
{
    if (!m_currentGrade.isEmpty()) {
        onGradeChanged(m_currentGrade);
    }
}

void BaseTab::setGrade(const QString &grade)
{
    if (grade.isEmpty())
        return;

    if (m_currentGrade != grade) {
        m_currentGrade = grade;
        onGradeChanged(grade);
    } else {
        refresh();
    }
}

void BaseTab::ensureGradeAndShow()
{
    QString grade = currentGrade();
    if (grade.isEmpty() && manager()) {
        grade = manager()->currentGrade();
    }
    if (grade.isEmpty()) {
        return; // 连默认年级都没有，啥也不做
    }

    if (currentGrade().isEmpty()) {
        // 首次设置
        m_currentGrade = grade;
        onGradeChanged(grade);
    } else {
        // 后续刷新
        refresh();
    }
}

void BaseTab::handleGradeChanged(const QString &grade)
{
    if (grade.isEmpty())
        return;

    if (m_currentGrade == grade) {
        return; // 没变，不重复调用
    }

    m_currentGrade = grade;
    onGradeChanged(grade);
}