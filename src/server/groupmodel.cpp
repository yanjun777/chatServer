#include "groupmodel.hpp"
#include "db.hpp" 

// 创建群组 
bool GroupModel::createGroup(Group& group){
    // 组装sql语句 
    char sql[1024] = {0};
    sprintf(sql, "insert into allgroup (groupname, groupdesc) values ('%s', '%s')"
        , group.getName().c_str(), group.getDesc().c_str());
    MySQL mysql; 
    if (mysql.connect()){
        if (mysql.update(sql)){
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

//加入群组
bool GroupModel::addGroup(int userid, int groupid, string role){
    // 组装sql语句 
    char sql[1024] = {0};
    sprintf(sql, "insert into groupuser (userid, groupid, role) values (%d, %d, '%s')", userid, groupid, role.c_str());
    MySQL mysql; 
    if (mysql.connect()){
        mysql.update(sql); 
        return true;
    }
    return false;
}

//查询用户所在群组信息
vector<Group> GroupModel::queryGroups(int userid,int groupid){
    // 组装sql语句 
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.groupname, a.groupdesc from allgroup a inner join \
        groupuser b on a.id = b.groupid where b.userid = %d", userid);
    MySQL mysql; 
    vector<Group> vec; 
    if (mysql.connect()){
        MYSQL_RES* res = mysql.query(sql); //查询结果集 
        if (res != nullptr){
            MYSQL_ROW row;  // 获取结果集中的行  
            while ((row = mysql_fetch_row(res)) != nullptr){
                Group group; 
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                vec.push_back(group); 
            }
            //释放资源
            mysql_free_result(res); 
        }
    }
    // 组员信息：
    for(Group& group : vec){
        char sql[1024] = {0};
        sprintf(sql, "select a.id, a.name, a.state,b.grouprole from user a inner join \
            groupuser b on a.id = b.userid where b.groupid = %d", group.getId());
        MYSQL_RES* res = mysql.query(sql); 
        if (res != nullptr){
            MYSQL_ROW row; 
            while ((row = mysql_fetch_row(res)) != nullptr){
                GroupUser user; // 组员信息 
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user); 
            }
            //释放资源
            mysql_free_result(res); 
        }
    }

    return vec;
}
