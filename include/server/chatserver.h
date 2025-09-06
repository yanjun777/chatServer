#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

class ChatServer{
public:
    ChatServer(EventLoop* loop, 
        const InetAddress& listenAddr, 
        const string& nameArg);
    ~ChatServer();

    void start();


private:

    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);

    TcpServer _server; // 组合的muduo库中的TcpServer 对象 
    EventLoop* _loop; // 事件循环

};


#endif




