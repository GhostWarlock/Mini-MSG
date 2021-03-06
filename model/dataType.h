//
// Created by wusong on 2021/2/14.
//

#ifndef MINI_MSG_DATATYPE_H
#define MINI_MSG_DATATYPE_H

#include <vector>
#include <QString>
#include <QPixmap>

/* login status */
typedef enum
{
    ONLINE = 0,
    ACTIVE,
    AWAY,
    BUSY,
    NOT_DISTURB,
    HIDE,
    OFFLINE
}loginState;

typedef std::vector<QString> StringVector;
typedef std::vector<QPixmap> ImageVector;
typedef std::vector<bool>    BoolVector;
typedef std::vector<int>    IntVector;
typedef std::vector<loginState> EnumVector;

typedef struct {
    IntVector ids;
    StringVector usrNameList;
    StringVector accountList;
    QString pwd;
    ImageVector headVector;
    BoolVector  rememberVector;
    bool isAutoLogin;
    EnumVector states;
}accountDataGroup;

typedef struct {
    int id;
    QString usrName;
    QString account;
    QPixmap head;
    QString passWord;
    bool isRemember = false;
    bool isAutoLogin = false;
    loginState state;
}accountInfo;



#endif //MINI_MSG_DATATYPE_H
