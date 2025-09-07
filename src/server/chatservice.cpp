#include "chatservice.hpp"
#include "public.hpp"
#include <iostream>
#include <muduo/base/Logging.h>


using namespace std;
using namespace muduo; 

ChatService* ChatService::getInstance()
{
    static ChatService service; //线程安全 
    return &service;
}

ChatService::ChatService()
{
    _handlerMap[static_cast<int>(EnMsgType::LOGIN_MSG)] = std::bind(&ChatService::login, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    _handlerMap[static_cast<int>(EnMsgType::REG_MSG)] = std::bind(&ChatService::reg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    
}

ChatService::~ChatService()
{

}

void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    cout << "login" << endl;
    
}

void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    cout << "reg" << endl;
    string name = js["name"];
    string password = js["password"];

    User user;
    user.setName(name);
    user.setPassword(password);
    user.setState("online");
    bool ret = _userModel.insert(user);
    if (ret){
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0; // 注册成功 
        response["id"] = user.getId();   
        conn->send(response.dump());
        cout << "reg success" << endl; 
    }else{
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1; // 注册失败 
        response["errmsg"] = "reg failed";
        conn->send(response.dump());
        cout << "reg failed" << endl; 
    }
}   

MsgHandler ChatService::getHandler(int msgType){

    auto it = _handlerMap.find(msgType); 
    if (it != _handlerMap.end()){
        return it->second;
    }else{
        // enldxxx
        // LOG_ERROR << "msgType: " << msgType << " not found"; 
        string str = "msgType: " + to_string(msgType) + " not found";

        return [=](const TcpConnectionPtr& conn, json& js, Timestamp time){
            LOG_ERROR << str;
        };
    }
}