#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


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
    void on_btnAddGrade_clicked();
    void on_btnDelGrade_clicked();
    void onTestStarted();
    void onTestFinished();

private:
    Ui::MainWindow *ui;
    bool m_testInProgress = false;

    WordBookTab   *wordBook    = nullptr;
    ReviewTab     *reviewTab   = nullptr;
    DictationTab  *dictationTab = nullptr;
};
#endif // MAINWINDOW_H