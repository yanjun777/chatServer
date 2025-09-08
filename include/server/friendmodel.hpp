#ifndef FRIENDMODEL_H  
#define FRIENDMODEL_H  
#include <vector> 
#include "user.hpp"
#include "db.hpp" 

// 提供好友表的添加、删除、查询接口  
class FriendModel{
public:
    // 添加好友关系 
    void insert(int userid, int friendid); 
    // 删除好友关系 
    void remove(int userid, int friendid); 
    // 查询好友关系 
    std::vector<User> query(int userid); 
}; 

#endif  