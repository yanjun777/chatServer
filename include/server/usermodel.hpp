#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "user.hpp"
// User表的增删改查 
class UserModel{
public:
    UserModel();
    ~UserModel();

    bool insert(User &user);
    bool updateState(User &user);
    User query(int id);
    bool remove(int id); 
};

#endif