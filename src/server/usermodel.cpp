#include "usermodel.hpp"
#include "db/db.hpp"
#include <iostream>

using namespace std;


UserModel::UserModel(){
    // 初始化数据库连接
    
}

UserModel::~UserModel(){
    // 关闭数据库连接
}
// 增删改查  
bool UserModel::insert(User &user){
    // 插入用户
    // 1. 组装sql语句
    // 2. 执行sql语句
    // 3. 返回结果

    char sql[1024] = {0};
    sprintf(sql,"insert into user (name, password, state) values ('%s', '%s', '%s')",
            user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str());
    MySQL mysql;
    if (mysql.connect()){
        if (mysql.update(sql)){
            // 获取插入的id 
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}   

User UserModel::query(int id){
    // 查询用户
    // 1. 组装sql语句
    // 2. 执行sql语句
    // 3. 返回结果
    char sql[1024] = {0};
    sprintf(sql,"select * from user where id = %d", id);
    MySQL mysql; // 创建数据库连接  

    if (mysql.connect()){
        MYSQL_RES* res = mysql.query(sql); // 执行sql语句 返回结果集  
        if (res != nullptr){
            MYSQL_ROW row = mysql_fetch_row(res); // 获取结果集  
            if (row != nullptr){
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                // 释放结果集 
                mysql_free_result(res); 
                return user;
            }
        }
    }
    return User();
}
// 更新用户状态 
bool UserModel::updateState(User &user){
    // 更新用户状态
    char sql[1024] = {0};
    sprintf(sql,"update user set state = '%s' where id = %d", user.getState().c_str(), user.getId());
    MySQL mysql; // 创建数据库连接  更新 查询 
    if (mysql.connect()){
        if (mysql.update(sql)){
            return true;
        }
    }
    return false;
}


void UserModel::resetState(){ 

    char sql[1024] = {0};
    sprintf(sql,"update user set state = 'offline' where state = 'online'");
    MySQL mysql; // 创建数据库连接  更新 查询 
    if (mysql.connect()){
        if (mysql.update(sql)){
            return ;
        }
    }
    return ;
}