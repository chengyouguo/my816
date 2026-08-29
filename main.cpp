#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // ✅ 建议加上这两行，建立专属配置空间
    QCoreApplication::setOrganizationName("Mybaelpnm");       // 换成你的组织名
    QCoreApplication::setApplicationName("GradeWordApp"); // 换成你的应用名
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}