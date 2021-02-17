//
// Created by wusong on 2021/2/13.
//

#ifndef MINI_MSG_USRMGT_H
#define MINI_MSG_USRMGT_H

#include "dataType.h"
#include "sqlite3/sqlite3.h"

using std::string;

class usrMgt {

public:
    explicit usrMgt();
    accountDataGroup getAccountInfo(int &status);
    int addAccount(const accountInfo &account);
    int deleteAccount(const int &id);
    string getErrMsg() const;

    ~usrMgt();

private:
    int creatTable(const string &dataBasePath);
    int selectTableAll();
private:
    sqlite3 *db = nullptr;
    char * errMsg = nullptr;
};


#endif //MINI_MSG_USRMGT_H
