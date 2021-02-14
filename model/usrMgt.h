//
// Created by wusong on 2021/2/13.
//

#ifndef MINI_MSG_USRMGT_H
#define MINI_MSG_USRMGT_H

#include "dataType.h"
#include "sqlite3/sqlite3.h"


class usrMgt {

public:
    explicit usrMgt();
    accountDataGroup getAccountInfo(int &status);
    int addAccount(accountInfo &account);
    int deleteAccount(const int &id);
    ~usrMgt();

private:
    int creatTable(const std::string &dataBasePath, const std::string &table);

private:
    sqlite3 *db = nullptr;
    char * errMsg = nullptr;
};


#endif //MINI_MSG_USRMGT_H
