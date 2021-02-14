//
// Created by wusong on 2021/2/14.
//

#ifndef MINI_MSG_SQLSENTENCE_H
#define MINI_MSG_SQLSENTENCE_H

#define accountInfoTable  "accountInfo (\
                                    id integer primary key autoincrement,\
                                    account varchar(20),\
                                    usrName varchar(20),\
                                    head varbinary,\
                                    password varchar(16),\
                                    isAutoLogin integer,\
                                    isRemember integet);"

#endif //MINI_MSG_SQLSENTENCE_H
