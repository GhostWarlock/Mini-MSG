//
// Created by wusong on 2021/2/9.
//

#include <QIcon>
#include "accountItem.h"
#include "resources.h"
#include <QDebug>

accountItem::accountItem(QWidget *parent)
            : QWidget(parent){
    setWindowFlag(Qt::FramelessWindowHint);
    name = new QLabel(this);
    account = new QLabel(this);
    head = new QLabel(this);
    closeButton = new QPushButton(QIcon(close_png),"", this);
    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();

    setFixedSize(190,55);
    head->setFixedSize(30,30);
    head->setScaledContents(true);
    name->setFixedSize(120,24);
    account->setFixedSize(120,24);
    closeButton->setFixedSize(25,25);
    closeButton->setIcon(QIcon(close_png));
    closeButton->setIconSize(QSize(25,25));
    closeButton->setStyleSheet(closeBtnStyle);

    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();
    vLayout->addWidget(name);
    vLayout->addStretch();
    vLayout->addWidget(account);
    hLayout->addWidget(head);
    hLayout->addItem(vLayout);
    hLayout->addWidget(closeButton);
    setLayout(hLayout);
    this->setStyleSheet(defaultThem);
    this->setAttribute(Qt::WA_StyledBackground,true);
    installEventFilter(this);

    connect(closeButton,&QPushButton::clicked, this, &accountItem::onRemoveItem);
}

accountItem::~accountItem() {
    delete name;
    delete account;
    delete head;
    delete closeButton;
//    delete vLayout;
    delete hLayout;
}

accountItem::accountItem(int id,const QPixmap &pixHead, const QString &nameStr,
                         const QString &accountStr, QWidget *parent)
            : QWidget(parent)
            ,itemId(id){
    setWindowFlag(Qt::FramelessWindowHint);

    name = new QLabel(this);
    account = new QLabel(this);
    head = new QLabel(this);
    closeButton = new QPushButton(QIcon(close_png),"", this);

    name->setText(nameStr);
    account->setText(accountStr);
    head->setPixmap(pixHead);

    setFixedSize(190,55);
    head->setFixedSize(30,30);
    head->setScaledContents(true);
    name->setFixedSize(120,22);
    account->setFixedSize(120,22);
    closeButton->setFixedSize(25,25);
    closeButton->setIcon(QIcon(close_png));
    closeButton->setIconSize(QSize(25,25));
    closeButton->setStyleSheet(closeBtnStyle);

    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();
    vLayout->addWidget(name);
    vLayout->addStretch();
    vLayout->addWidget(account);
    hLayout->addWidget(head);
    hLayout->addItem(vLayout);
    hLayout->addWidget(closeButton);
    setLayout(hLayout);
    this->setStyleSheet(defaultThem);
    this->setAttribute(Qt::WA_StyledBackground,true);
    installEventFilter(this);

    connect(closeButton,&QPushButton::clicked, this, &accountItem::onRemoveItem);
}

void accountItem::setAccountNum(const QString &number) {
    account->setText(number);
}

QString accountItem::getAccountNum() const {
    return account->text();
}

void accountItem::setNickName(const QString &nickName) {
    name->setText(nickName);
}

QString accountItem::getNickName() const {
    return name->text();
}

void accountItem::setHead(const QPixmap &pixHead) {
    head->setPixmap(pixHead);
}

void accountItem::onRemoveItem() {
    emit sigRemoveItem(account->text());
}

void accountItem::mousePressEvent(QMouseEvent *event) {
    isSelect = true;
}

void accountItem::mouseReleaseEvent(QMouseEvent *event) {
    if(isSelect){
        emit sigShowItem(account->text());
    }
}

bool accountItem::eventFilter(QObject *target, QEvent *event) {
    if(target == this){
        if(event->type() == QEvent::Enter){
            this->setStyleSheet(hoverThem);
            head->setFixedSize(35,35);
        }
        else if(event->type() == QEvent::Leave){
            this->setStyleSheet(defaultThem);
            head->setFixedSize(30,30);
        }
    }
    return QObject::eventFilter(target, event);
}

int accountItem::getAccountId() const {
    return itemId;
}



