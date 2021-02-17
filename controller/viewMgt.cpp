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

//    accountsData.usrNameList.push_back("ZhangSan");
//    accountsData.usrNameList.push_back("LiSi");
//    accountsData.accountList.push_back("15957196842");
//    accountsData.accountList.push_back("13989498732");
//
//    accountsData.rememberVector.push_back(true);
//    accountsData.rememberVector.push_back(false);
//    accountsData.headVector.push_back(QPixmap(help_png));
//    accountsData.headVector.push_back(QPixmap(addUsr_png));
//    accountsData.pwd = normalPwd;
//    accountsData.states.push_back(ACTIVE);
//    accountsData.states.push_back(BUSY);
//    accountsData.isAutoLogin = false;

    {   /****  for login ****/
        int status = 0;
        accountsData = pUsrMgt->getAccountInfo(status);
        loginView = new loginWindow(nullptr,accountsData);
        // connect loginWindow signals to this slots this eventLoop
        connect(loginView,&loginWindow::sigExitLoginWindow,viewEvent,&QEventLoop::quit);
        connect(loginView,&loginWindow::sigLoginRequest, this,&viewMgt::onLoginRequest);
        connect(loginView,&loginWindow::sigCancelLogin,this,&viewMgt::onCancelLogin);
        connect(loginView,&loginWindow::sigDeleteAccount, this,[&](const int &id){
            auto res = pUsrMgt->deleteAccount(id);   // maybe fail
            if(res){
                qDebug() << "delete account fail, id = " << id << ";";
                qDebug() << "error message:" << pUsrMgt->getErrMsg().data();
            }
            else
                qDebug() << "delete account success, id = " << id << ";";
        });
        connect(pUsrMgt, &usrMgt::sigLoginResult, loginView,&loginWindow::onLoginState);
        loginView->show();
        viewEvent->exec();      // wait for loginView quite
        loginView->close();
        delete loginView;
    }

    { /**** for msg ****/


    }





}

viewMgt::viewMgt(QObject *parent)
            : QObject(parent)
            , pUsrMgt(new usrMgt){

}

void viewMgt::onLoginRequest(const accountInfo &data) {

    qDebug() << "get login Request";
//    qDebug() << "account:" << data.account;
//
//    qDebug() << "password:" << data.passWord;
//
//    qDebug() << "autoLogin?:" << data.isAutoLogin;
//
//    qDebug() << "remember?:" << data.isRemember;
    pUsrMgt->clientLogin(data);
//    pUsrMgt->addAccount(data);
}

void viewMgt::onCancelLogin() {
    qDebug() << "get cancel login";
}
