//
// Created by wusong on 2021/2/6.
//

#include "controller/viewMgt.h"
#include "loginwindow.h"
#include "resources.h"

#include <QDebug>


void viewMgt::start() {
    if(viewEvent == nullptr){;
        viewEvent = new QEventLoop(this);
    }
    loginDataGroup accountsData;
    accountsData.usrNameList.append("15957196842");
    accountsData.usrNameList.append("13989498732");
    accountsData.rememberVector.push_back(true);
    accountsData.rememberVector.push_back(false);
    accountsData.headVector.push_back(QPixmap(help_png));
    accountsData.headVector.push_back(QPixmap(addUsr_png));
    accountsData.pwd = normalPwd;
    accountsData.state = ACTIVE;
    accountsData.isAutoLogin = true;
    // load setting

    loginWindow loginView(nullptr,accountsData);

    // connect LoginWindow signals to this slots this eventLoop

    loginView.show();
    connect(&loginView,&loginWindow::exitLoginWindow,viewEvent,&QEventLoop::quit);
    qDebug() << "show";
    viewEvent->exec();      // wait for loginView quite
    loginView.close();
    qDebug() << "login finish";
}

viewMgt::viewMgt() {
    ;
}
