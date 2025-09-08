#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"

class GroupModel{
public:
    GroupModel(){}
    ~GroupModel(){}
    // 创建群组 
    bool createGroup(Group& group);
    // 加入群组 
    bool addGroup(int userid, int groupid, string role);
    // 查询用户所在群组信息 
    vector<Group> queryGroups(int userid,int groupid);
    // 查询群组用户信息 //
    vector<int> queryGroupUsers(int userid,int groupid);
};

#endif  