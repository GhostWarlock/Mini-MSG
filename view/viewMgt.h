//
// Created by wusong on 2021/2/6.
//

#ifndef MINI_MSG_VIEWMGT_H
#define MINI_MSG_VIEWMGT_H
#include <QWidget>
#include <QEventLoop>

class viewMgt : public QObject {
    Q_OBJECT
public:
    explicit viewMgt();
    void start();

public slots:

private:
    QEventLoop* viewEvent = nullptr;

};


#endif //MINI_MSG_VIEWMGT_H
