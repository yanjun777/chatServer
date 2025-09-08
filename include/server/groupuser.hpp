#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

class GroupUser : public User{
public:
    GroupUser(){}
    ~GroupUser(){}

    void setRole(string role){ this->_role = role; }
    string getRole(){ return this->_role; }

private:
    string _role; 
};

#endif  