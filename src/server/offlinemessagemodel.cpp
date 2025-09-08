#include "offlinemessagemodel.hpp"

OfflineMessageModel::OfflineMessageModel()
{
}

OfflineMessageModel::~OfflineMessageModel()
{
}

void OfflineMessageModel::insert(int userid, string msg)
{

    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage (userid, message) values (%d, '%s')", userid, msg.c_str());
    MySQL mysql;
    if (mysql.connect()){
        if (mysql.update(sql)){
            return;
        }
    }
}

void OfflineMessageModel::remove(int userid)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid = %d", userid);
    MySQL mysql;
    if (mysql.connect()){
        if (mysql.update(sql)){
            return;
        }
    }
}

vector<string> OfflineMessageModel::query(int userid)
{
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userid = %d", userid);
    MySQL mysql;
    vector<string> msgList;
    // 连接数据库
    if (mysql.connect()){
        // 查询数据库 
        MYSQL_RES* res = mysql.query(sql);
        if (res){
            // 遍历结果集
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr){
                msgList.push_back(row[0]);
            }
            // 释放结果集
            mysql_free_result(res); 
        }   
    }
    return msgList;
}
