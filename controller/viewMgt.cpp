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
    accountsData.usrNameList.push_back("ZhangSan");
    accountsData.usrNameList.push_back("LiSi");
    accountsData.accountList.push_back("15957196842");
    accountsData.accountList.push_back("13989498732");

    accountsData.rememberVector.push_back(true);
    accountsData.rememberVector.push_back(false);
    accountsData.headVector.push_back(QPixmap(help_png));
    accountsData.headVector.push_back(QPixmap(addUsr_png));
    accountsData.pwd = normalPwd;
    accountsData.state = ACTIVE;
    accountsData.isAutoLogin = true;

    loginWindow loginView(nullptr,accountsData);

    // connect LoginWindow signals to this slots this eventLoop
    connect(&loginView,&loginWindow::sigExitLoginWindow,viewEvent,&QEventLoop::quit);
    connect(&loginView,&loginWindow::sigLoginRequest, this,&viewMgt::onLoginRequest);
    connect(&loginView,&loginWindow::sigCancelLogin,this,&viewMgt::onCancelLogin);

    loginView.show();
    qDebug() << "show";
    viewEvent->exec();      // wait for loginView quite
    loginView.close();
    qDebug() << "login finish";
}

viewMgt::viewMgt() {
    ;
}

void viewMgt::onLoginRequest(const loginDataGroup &data) {

    qDebug() << "get login Request";
    qDebug() << "account:" << data.accountList[0];

    qDebug() << "password:" << data.pwd;

    qDebug() << "autoLogin?:" << data.isAutoLogin;

    qDebug() << "remember?:" << data.rememberVector[0];
}

void viewMgt::onCancelLogin() {
    qDebug() << "get cancel login";
}
