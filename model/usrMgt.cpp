//
// Created by wusong on 2021/2/13.
//

#include "usrMgt.h"
#include "sqlSentence.h"

#include <QDebug>
#include <QCryptographicHash>
#include <thread>

usrMgt::usrMgt() {

    auto res = creatTable(string (appPath)+string(dataBase));
    if(res)
        qDebug() << "create fail" ;
    else
        qDebug() << "create success" ;
//    qDebug() << QCryptographicHash::hashLength(QCryptographicHash::Md5);
//    auto data = (QCryptographicHash::hash("13",QCryptographicHash::Md5).toHex());
//    qDebug() << QString(data).length();
//    qDebug() << QString(data);
}

int usrMgt::creatTable(const string &dataBasePath) {
    auto res = sqlite3_open_v2(dataBasePath.c_str(),&db,
                          SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
                               nullptr);
    if(res == 0) {
        // check table status. if not exists. create
        std::string sql = "create table if not exists ";
        sql += accountTableName;
        sql += accountTableContent;
        delete[] errMsg;
        res = sqlite3_exec(db,sql.c_str(), nullptr, nullptr,&errMsg);
    }
    return res;
}

int usrMgt::selectTableAll() {
    std::string sql = "select * from";
    sql += accountTableName;
    delete[] errMsg;
    auto res = sqlite3_exec(db,sql.c_str(), nullptr, nullptr,&errMsg);
    return res;
}

accountDataGroup usrMgt::getAccountInfo(int &status) {
    accountDataGroup data;
    std::string sql = "select * from ";
    sql += accountTableName;
    char** pResult;
    int nRow = 0;
    int nCol = 0;
    delete[] errMsg;
    auto res = sqlite3_get_table(db,sql.c_str(),&pResult,&nRow,&nCol,&errMsg);
    if(res == 0 && nCol == accountTableCols){
        data.ids.clear();
        data.usrNameList.clear();
        data.accountList.clear();
        data.rememberVector.clear();
        for(int i=1; i<nRow+1; ++i){
            data.ids.push_back(strtol(pResult[accountTableCols*i], nullptr,0));
            data.accountList.push_back(pResult[accountTableCols*i + 1]);
            data.usrNameList.push_back(pResult[accountTableCols*i + 2]);
            data.headVector.push_back(QPixmap(pResult[accountTableCols*i + 3]));
            data.rememberVector.push_back(string(pResult[accountTableCols * i + 6]) == "1");
            data.states.push_back(loginState(strtol(pResult[accountTableCols*i + 7], nullptr,0)));
        }
        data.isAutoLogin = string(pResult[accountTableCols + 5]) == "1";
    }
    else status = -1;
    sqlite3_free_table(pResult);
    return data;
}

int usrMgt::addAccount(const accountInfo &account) {
    string sql = "insert into ";
    sql += accountTableName;
    sql += " (account, usrName, headPath, password, isAutoLogin, isRemember, loginState)";
    sql += " values(\'" + account.account.toStdString() + "\', ";
    sql += ("\'" + account.usrName.toStdString()+"\' , ");
    auto headPath = QString(appPath) + QString(usrHeadImg);
    headPath += ((account.usrName.isEmpty() ? QString("avatar") : account.usrName) + ".png");
    qDebug() << headPath;
    account.head.save(headPath);
    sql += ("\'" + headPath.toStdString()  + "\' , ");
    sql += ("\'" + account.passWord.toStdString()+"\' , ");   // 32byte MD5 value
    sql += (account.isAutoLogin ? "1" : "0");
    sql += ", ";
    sql += (account.isRemember ? "1" : "0");
    sql += ", ";
    sql += (std::to_string(account.state % 7) + ");") ;
    delete[] errMsg;
    auto res = sqlite3_exec(db,sql.c_str(), nullptr, nullptr,&errMsg);
    return res;
}

int usrMgt::deleteAccount(const int &id) {
    string sql = "delete from ";
    sql += accountTableName;
    sql += " where id = ";
    sql += std::to_string(id);
    sql += ";";
    delete[] errMsg;
    auto res = sqlite3_exec(db,sql.c_str(), nullptr, nullptr,&errMsg);
    return res;
}

string usrMgt::getErrMsg() const {
    return string(errMsg);
}

usrMgt::~usrMgt() {
    sqlite3_close_v2(db);
    delete[] errMsg;
}

void usrMgt::clientLogin(const accountInfo &account) {

    auto loginThread = new std::thread([&]{
        int stateCode = 0;
        accountInfo curAccount = account;

        // login service


        curAccount.passWord = QCryptographicHash::hash(curAccount.passWord.toLocal8Bit(),QCryptographicHash::Md5).toHex();

        // save account info
        if(curAccount.id == -1){   // new account
            addAccount(curAccount);
        }
        else{                      // modify account
            modifyAccount(curAccount);
        }

        // send login result
        emit sigLoginResult("OK");
    });
    loginThread->detach();
}

int usrMgt::modifyAccount(const accountInfo &account) {
    string sql = "update ";
    sql += accountTableName;
    sql += (" set account=\'" + account.account.toStdString() + "\', ");
    sql += ("usrName=\'" + account.usrName.toStdString() + "\', ");
    auto headPath = QString(appPath) + QString(usrHeadImg);
    headPath += ((account.usrName.isEmpty() ? QString("avatar") : account.usrName) + ".png");
    account.head.save(headPath);
    sql += ("headPath=\'" + headPath.toStdString() + "\', ");
    sql += ("password=\'" + account.passWord.toStdString() + "\', ");
    sql += ("isAutoLogin=" + (account.isAutoLogin ? std::to_string(1) : std::to_string(0)) + ", ");
    sql += ("isRemember=" + (account.isRemember ? std::to_string(1) : std::to_string(0)) + ", ");
    sql += ("loginState=" + std::to_string(account.state % 7));
    sql += (" where id = " + std::to_string(account.id) + ";");
    delete[] errMsg;
    auto res = sqlite3_exec(db,sql.c_str(), nullptr, nullptr,&errMsg);
    return res;
}
