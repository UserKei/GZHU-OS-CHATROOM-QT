#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "loginwindow.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用信息
    app.setApplicationName("ChatRoom");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ChatRoom System");
    
    // 初始化数据库
    Database::instance().initDatabase();
    
    // 创建登录窗口
    LoginWindow loginWindow;
    loginWindow.show();
    
    return app.exec();
}
