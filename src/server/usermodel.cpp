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