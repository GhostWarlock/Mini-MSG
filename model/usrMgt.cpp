//
// Created by wusong on 2021/2/13.
//

#include "usrMgt.h"
#include "sqlSentence.h"

usrMgt::usrMgt() {


}

int usrMgt::creatTable(const std::string &dataBasePath, const std::string &table) {
    auto res = sqlite3_open_v2(dataBasePath.c_str(),&db,
                          SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
                               errMsg);
    if(res == 0) {
        // check table status. if not exists. create
        std::string sql = "create table if not exists";
        sql += table;
        res = sqlite3_exec(db,sql.c_str(), nullptr, nullptr,&errMsg);
    }
    return res;
}
