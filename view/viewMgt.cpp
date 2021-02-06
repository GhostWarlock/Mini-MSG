//
// Created by wusong on 2021/2/6.
//

#include "viewMgt.h"
#include "loginwindow.h"

#include <QDebug>


void viewMgt::start() {
    if(viewEvent == nullptr){;
        viewEvent = new QEventLoop(this);
    }
    loginDataGroup accountsData;
    // load setting

    LoginWindow loginView;

    // connect LoginWindow signals to this slots this eventLoop

    loginView.show();
    connect(&loginView,&LoginWindow::exitLoginWindow,viewEvent,&QEventLoop::quit);
    qDebug() << "show";
    viewEvent->exec();      // wait for loginView quite
    loginView.close();
    qDebug() << "login finish";
}

viewMgt::viewMgt() {
    ;
}
