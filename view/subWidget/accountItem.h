//
// Created by wusong on 2021/2/9.
//

#ifndef MINI_MSG_ACCOUNTITEM_H
#define MINI_MSG_ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

class accountItem : public QWidget{
Q_OBJECT
public:
    explicit accountItem(QWidget *parent = nullptr);
    accountItem(int id, const QPixmap &pixHead, const QString& nameStr, const QString& accountStr, QWidget *parent = nullptr);
    ~accountItem() override;

    void setAccountNum(const QString& number);
    QString getAccountNum() const;
    int getAccountId() const;

    void setNickName(const QString& nickName);
    QString getNickName() const;

    void setHead(const QPixmap &pixHead);

signals:
    void sigShowItem(const QString&);
    void sigRemoveItem(const QString&);

private slots:
    void onRemoveItem();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;

private:
    QLabel* name = nullptr;
    QLabel* account = nullptr;
    QLabel* head = nullptr;
    QPushButton* closeButton = nullptr;
    QVBoxLayout* vLayout = nullptr;
    QHBoxLayout* hLayout = nullptr;

    bool isSelect = false;
    int itemId = -1;
};

#define hoverThem "background-color: cornflowerblue;\nfont-size:16px;"
#define defaultThem "background-color: transparent;\nfont-size:12px;"


#define closeBtnStyle "QPushButton\
                        {\
                            background-color: transparent;\
                            border-style:outset;\
                            border-color:transparent;\
                        }\
                        QPushButton:hover\
                        {\
                            background-color: cornflowerblue;\
                            border-style:outset;\
                            border-width:0;\
                            border-radius:4px;\
                            border-color: transparent;\
                        }\
                        QPushButton:pressed\
                        {\
                            background-color: cadetblue;\
                            border-style:inset;\
                            border-width:0;\
                            border-radius:5px;\
                            border-color: transparent;\
                        }"

#endif //MINI_MSG_ACCOUNTITEM_H
