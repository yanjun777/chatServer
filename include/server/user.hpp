#ifndef USER_HPP
#define USER_HPP

#include <string>

using namespace std;
// 在线用户表 ORM 对象关系映射 todo 
class User{
public:
    User(int id=-1, string name="", string password="", string state="offline"){
        this->_id = id;
        this->_name = name;
        this->_password = password;
        this->_state = state;
    }
    ~User(){}

    void setId(int id){ this->_id = id; }
    void setName(string name){ this->_name = name; }
    void setPassword(string password){ this->_password = password; }
    void setState(string state){ this->_state = state; }

    int getId(){ return this->_id; }
    string getName(){ return this->_name; }
    string getPassword(){ return this->_password; }
    string getState(){ return this->_state; }

private:
    int _id;
    string _name;
    string _password;
    string _state; 
};

#endif