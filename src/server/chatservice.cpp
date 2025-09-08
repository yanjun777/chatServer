#include "chatservice.hpp"
#include "public.hpp"
#include <iostream>
#include <muduo/base/Logging.h>
#include <vector>

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
    _handlerMap[static_cast<int>(EnMsgType::ONE_CHAT_MSG)] = std::bind(&ChatService::oneChat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    _handlerMap[static_cast<int>(EnMsgType::ADD_FRIEND_MSG)] = std::bind(&ChatService::addFriend, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

ChatService::~ChatService()
{

}

void ChatService::reset()
{
    // 重置用户状态 
    _userModel.resetState(); 
} 

void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    cout << "login" << endl;
    int id = js["id"].get<int>(); 
    string password = js["password"];

    User user = _userModel.query(id);
    if (user.getId() == id && user.getPassword() == password){
        if (user.getState() == "online"){
            // 登录失败
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2; // 登录失败 
            response["errmsg"] = "user already login";
            conn->send(response.dump());
        }else{
            // 登录成功，记录用户连接信息 
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap[id] = conn;
            } 

            // 登录成功
            user.setState("online");
            _userModel.updateState(user); 

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0; // 登录成功 
            response["id"] = user.getId();   
            response["name"] = user.getName(); 
            // 查询该用户是否有离线消息
            vector<string> offlineMsgList = _offlineMsgModel.query(id);
            if (!offlineMsgList.empty()){
                // 有离线消息，推送离线消息
                response["offlinemsg"] = offlineMsgList;
                // 删除该用户离线消息
                _offlineMsgModel.remove(id); 
            }

            // 查询好友信息 
            vector<User> friendList = _friendModel.query(id);
            if (!friendList.empty()){
                vector<string> vec; 
                for (User& user : friendList){
                    json js; 
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec.push_back(js.dump());
                }
                response["friends"] = vec;
            }
            

            conn->send(response.dump());
            cout << "login success" << endl;  
        }       
    }else{
        // 登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1; // 登录失败 
        response["errmsg"] = "login failed";
        conn->send(response.dump());
        cout << "login failed" << endl; 
    }
    
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

void ChatService::clientCloseException(const TcpConnectionPtr& conn){
    // 删除用户连接信息 
    // 更新用户状态  
    User user;
    {
        lock_guard<mutex> lock(_connMutex);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it){
            if (it->second == conn){
                user.setId(it->first);
                
                _userConnMap.erase(it);
                break;
            }
        }
    }
    if(user.getId() != -1){
        // 更新用户状态  数据库连接操作与连接删除操作分离 
        user.setState("offline"); 
        _userModel.updateState(user); 
    }
}

void ChatService::oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int toId = js["to"].get<int>();
    
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toId);
        if (it != _userConnMap.end()){
            //服务器主动推送消息给toId用户 
            it->second->send(js.dump());
            return; 
        }
    }
    // 否则不在线 
    _offlineMsgModel.insert(toId, js.dump()); 
    
    
}

// 处理添加好友业务  
void ChatService::addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time){
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    // 存储好友信息 
    _friendModel.insert(userid, friendid);


}