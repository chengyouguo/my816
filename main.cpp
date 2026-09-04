#include "mainwindow.h"



#include <QApplication>
#include <QMainWindow>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    // ✅ 建议加上这两行，建立专属配置空间
    QCoreApplication::setOrganizationName("Mybaelpnm");       // 换成你的组织名
    QCoreApplication::setApplicationName("GradeWordApp"); // 换成你的应用名

    QApplication a(argc, argv);
    a.setApplicationName("WordApp");

    const QString serverName = "WordApp-SingleInstance";

    // 尝试连接已有实例
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500)) {
        QTextStream stream(&socket);
        stream << "activate\n";
        stream.flush();
        socket.waitForBytesWritten();
        socket.close();

        QMessageBox::information(nullptr,
                                 "提示",
                                 "程序已经在运行中，不能重复打开。");
        return 0;
    }

    // 清理崩溃残留 + 创建本地 server
    QLocalServer::removeServer(serverName);
    QLocalServer server;
    server.listen(serverName);

    // 已有实例收到新连接时激活窗口
    QObject::connect(&server, &QLocalServer::newConnection, [&]() {
        QLocalSocket *client = server.nextPendingConnection();
        client->deleteLater();
        // 激活主窗口
        for (QWidget *w : QApplication::topLevelWidgets()) {
            if (QMainWindow *mw = qobject_cast<QMainWindow *>(w)) {
                mw->show();
                mw->raise();
                mw->activateWindow();
            }
        }
    });

    // === 你的正常启动逻辑 ===
   // QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}