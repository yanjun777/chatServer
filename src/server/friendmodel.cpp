#include "friendmodel.hpp"

// 添加好友关系 
void FriendModel::insert(int userid, int friendid){
    // 组装sql语句 
    char sql[1024] = {0};
    sprintf(sql, "insert into friend (userid, friendid) values (%d, %d)", userid, friendid);
    MySQL mysql; 
    if (mysql.connect()){
        mysql.update(sql); 
    }
    // 其实这里friend表 应该要有外键约束，但是为了简单，这里就不加了 
}

vector<User> FriendModel::query(int userid){
    // 组装sql语句 
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.name, a.state from user a inner join friend b on b.friendid = a.id where b.userid = %d", userid);
    MySQL mysql; 
    vector<User> vec; 
    if (mysql.connect()){
        MYSQL_RES* res = mysql.query(sql); 
        if (res != nullptr){
            MYSQL_ROW row; 
            while ((row = mysql_fetch_row(res)) != nullptr){
                User user; 
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user); 
            }
            mysql_free_result(res);  
        }
    }
    return vec; 
}

void FriendModel::remove(int userid, int friendid){
    // 组装sql语句 
    char sql[1024] = {0};
    sprintf(sql, "delete from friend where userid = %d and friendid = %d", userid, friendid);
    MySQL mysql; 
}