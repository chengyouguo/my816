#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class WordManager;
class WordBookTab;
class ReviewTab;
class DictationTab;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ✅ 年级管理（唯一允许改年级的地方）
    void on_btnAddGrade_clicked();
    void on_btnDelGrade_clicked();

private:
    Ui::MainWindow *ui;

    // ✅ 全局数据（唯一数据源）
    WordManager *m_mgr;
 QString m_pendingGrade;   // ✅ 你自己的
    // ✅ 三个业务 Tab（由主窗口创建 / 销毁，只读当前年级）
    WordBookTab   *wordBook;
    ReviewTab     *reviewTab;
    DictationTab  *dictationTab;
};

#endif // MAINWINDOW_H