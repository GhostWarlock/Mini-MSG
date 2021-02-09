//
// Created by wusong on 2021/2/9.
//

#ifndef MINI_MSG_ACCOUNTBOX_H
#define MINI_MSG_ACCOUNTBOX_H

#include <QComboBox>
#include <QListWidget>
#include "accountItem.h"


class accountBox : public QComboBox {

Q_OBJECT

public:
    explicit accountBox(QWidget *parent = nullptr);
    ~accountBox() override;

    void addAccount(accountItem* pAccountItem);

private slots:
    void onShowAccount(const QString&);
    void onRemoveAccount(const QString&);

private:
    QListWidget* pListWidget = nullptr;
};


#define thisStyle  "QComboBox\
                    {\
                    background:white;\
                    padding-left:5px ;\
                    border-top-left-radius:3px;\
                    border-top-right-radius:3px;\
                    border: 1px solid rgb(209 , 209 , 209);\
                    font-size: 16px;\
                    }\
                    QComboBox:hover\
                    {\
                    border: px solid rgb(21 , 131 , 221);\
                    }\
                    QComboBox QAbstractItemView::item\
                    {\
                            \
                            height: 55px;\
                    }\
                    QComboBox::down-arrow\
                    {\
                    border-image:url(:/resources/loginWindow/tool/dropDownButton.png) 0 34 0 0 ;\
                    }\
                    QComboBox::down-arrow:hover\
                    {\
                    border-image:url(:/resources/loginWindow/tool/dropDownButton.png) 0 17 0 17 ;\
                    }\
                    QComboBox::down-arrow:on\
                    {\
                    border-image:url(:/resources/loginWindow/tool/dropDownButton.png) 0 0 0 34 ;\
                    }\
                    QComboBox::drop-down\
                    {\
                    width:20px;\
                    background:transparent;\
                    padding-right:5px;\
                    }"


#endif //MINI_MSG_ACCOUNTBOX_H
