#ifndef CHATSERVICE_H 
#define CHATSERVICE_H 

#include <muduo/net/TcpServer.h>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include "public.h"
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json; 
using MsgHandler = std::function<void(const TcpConnectionPtr& conn, json& js, Timestamp time)>;

// singleton 单例模式 
class ChatService{
public:
    static ChatService* getInstance();
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time); 

    MsgHandler getHandler(int msgType);     
private:
    ChatService(); 
    ~ChatService(); 
    // 处理登录业务
    unordered_map<int, MsgHandler> _handlerMap; 


};





#endif 