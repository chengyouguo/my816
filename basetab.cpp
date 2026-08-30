#include "basetab.h"
#include <QDebug>

BaseTab::BaseTab(WordManager *mgr, QWidget *parent)
    : QWidget(parent), m_mgr(mgr)
{
    Q_ASSERT(mgr); // 防呆：没传 Manager 直接崩

    // ✅ 信号只在这里连一次
    connect(m_mgr, &WordManager::gradeChanged,
            this, &BaseTab::handleGradeChanged);

}

void BaseTab::handleGradeChanged(const QString &grade)
{
    if (grade.isEmpty())
        return;

    // ✅ 基类统一缓存
    m_currentGrade = grade;

    // ✅ 强制子类响应
   // onGradeChanged(grade);
  //  onGradeChanged(grade, false); // ✅ 切年级 = 静音
}