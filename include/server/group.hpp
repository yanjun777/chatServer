#ifndef GROUP_H
#define GROUP_H

#include "groupuser.hpp"
#include <vector> 
// 群组的ORM类  
class Group{
public:
    Group(int id=-1, std::string name="", std::string desc=""){}
    ~Group(){}

    void setId(int id){ this->_id = id; }
    void setName(std::string name){ this->_name = name; }
    void setDesc(std::string desc){ this->_desc = desc; }

    int getId(){ return this->_id; }
    std::string getName(){ return this->_name; }
    std::string getDesc(){ return this->_desc; }

    std::vector<GroupUser> &getUsers(){ return this->_users; }

private:
    int _id;
    std::string _name;
    std::string _desc; // 群描述  
    std::vector<GroupUser> _users;
};

#endif  