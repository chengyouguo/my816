#include "basetab.h"

BaseTab::BaseTab(QWidget *parent)
    : QWidget(parent)
    , m_currentGrade()
{
    connect(WordManager::instance(), &WordManager::gradeChanged,
            this, &BaseTab::handleGradeChanged);
}

void BaseTab::handleGradeChanged(const QString &grade)
{
    if (grade.isEmpty())
        return;
    setGrade(grade);
    onGradeChanged(grade);
}

void BaseTab::setGrade(const QString &grade)
{
    if (m_currentGrade != grade) {
        m_currentGrade = grade;
    }
}

void BaseTab::ensureGradeAndShow()
{
    if (m_currentGrade.isEmpty()) {
        QStringList gs = manager()->grades();
        if (!gs.isEmpty()) {
            setGrade(gs.first());
            onGradeChanged(m_currentGrade);
        }
    }
}

void BaseTab::refresh()
{
    onGradeChanged(m_currentGrade);
}