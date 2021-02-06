
#include <QApplication>
#include <QWidget>
#include "loginwindow.h"
int main(int argc, char *argv[])
{
    QApplication app_mini_msg(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/appIcon.png"));
    LoginWindow login;
    login.show();
    return QApplication::exec();   //QAppliction::exec() is a static func
}
