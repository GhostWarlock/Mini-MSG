//
// Created by wusong on 2021/2/6.
//

#ifndef MINI_MSG_VIEWMGT_H
#define MINI_MSG_VIEWMGT_H
#include <QWidget>
#include <QEventLoop>
#include "loginwindow.h"
#include "model/dataType.h"
#include "model/usrMgt.h"

class viewMgt : public QObject {
    Q_OBJECT
public:
    explicit viewMgt(QObject *parent = nullptr);
    void start();

public slots:
    void onLoginRequest(const accountInfo &);
    void onCancelLogin();

private:
    QEventLoop* viewEvent = nullptr;
    usrMgt* pUsrMgt = nullptr;
    accountDataGroup accountsData;


};


#endif //MINI_MSG_VIEWMGT_H
