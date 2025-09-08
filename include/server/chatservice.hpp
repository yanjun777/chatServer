#ifndef CHATSERVICE_H 
#define CHATSERVICE_H 

#include <muduo/net/TcpServer.h>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include "public.hpp"
#include "usermodel.hpp"
#include <mutex>
#include "offlinemessagemodel.hpp" 
#include "friendmodel.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json; 
using MsgHandler = std::function<void(const TcpConnectionPtr& conn, json& js, Timestamp time)>;

// singleton 单例模式 
class ChatService{
public:
    // 获取单例对象的接口函数 
    static ChatService* getInstance();
    // 处理登录业务 
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 处理注册业务 
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time); 
    // 处理客户端异常退出 
    void clientCloseException(const TcpConnectionPtr& conn);  
    // 处理一对一聊天业务 
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time); 
    // 处理添加好友业务 
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time); 
    // 服务器已拆毁功能，业务重置函数
    void reset();   
    
    // 获取消息对应的处理器 
    MsgHandler getHandler(int msgType);     
private:
    // 单例模式 
    ChatService(); 
    ~ChatService(); 

    // 处理登录业务
    unordered_map<int, MsgHandler> _handlerMap; 

    // 存储用户连接的socket 和 用户id 的映射 
    unordered_map<int, TcpConnectionPtr> _userConnMap;  
    // 连接存储线程互斥锁 
    mutex _connMutex;  

    // 数据操作类
    UserModel _userModel; 
    OfflineMessageModel _offlineMsgModel;  
    FriendModel _friendModel; 
};





#endif 