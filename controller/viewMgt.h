//
// Created by wusong on 2021/2/6.
//

#ifndef MINI_MSG_VIEWMGT_H
#define MINI_MSG_VIEWMGT_H
#include <QWidget>
#include <QEventLoop>
#include "loginwindow.h"

class viewMgt : public QObject {
    Q_OBJECT
public:
    explicit viewMgt();
    void start();

public slots:
    void onLoginRequest(const loginDataGroup &);
    void onCancelLogin();

private:
    QEventLoop* viewEvent = nullptr;

};


#endif //MINI_MSG_VIEWMGT_H
