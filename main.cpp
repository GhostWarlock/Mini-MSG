

#include <QIcon>
#include <thread>
#include <QDebug>
#include <QApplication>
#include "resources.h"
#include "controller/viewMgt.h"

int main(int argc, char *argv[])
{
    QApplication app_mini_msg(argc, argv);
    QApplication::setWindowIcon(QIcon(appIcon_png));
    auto viewManagement = new viewMgt;
    // controller thread

    // model thread

    // start UI mgt
    viewManagement->start();



    return 0;
//    return QApplication::exec();   //QAppliction::exec() is a static func
}
