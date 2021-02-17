//
// Created by wusong on 2021/2/14.
//

#ifndef MINI_MSG_SQLSENTENCE_H
#define MINI_MSG_SQLSENTENCE_H

#ifdef Q_OS_WIN

#define dataBase  // windows path
#endif
#ifdef Q_OS_LINUX
//#define dataBase  "/home/wusong/Mini-MSG/database/miniMsg.db"
#define appPath "/home/wusong/Mini-MSG/"
#endif
#define usrHeadImg "usrHeadImages/"
#define users      "users/"
#define dataBase "database/miniMsg.db"

#define accountTableName     "accountInfo"
#define accountTableContent  "(id integer primary key autoincrement,\
                                account varchar(20) not null,\r\
                                usrName varchar(20),\r\
                                headPath varchar(255),\r\
                                password char(32),\r\
                                isAutoLogin integer not null,\r\
                                isRemember integet not null,\r\
                                loginState integet not null);"
#define accountTableCols    8
#define selectAllFrom       "select * from"

#endif //MINI_MSG_SQLSENTENCE_H
