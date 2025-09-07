#include "chatserver.hpp"
#include "chatservice.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop, 
    const InetAddress& listenAddr, 
    const string& nameArg)
    : _server(loop, listenAddr, nameArg)
{
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    _server.setThreadNum(4);
}

ChatServer::~ChatServer()
{

}

void ChatServer::start()
{
    _server.start();

}

void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        cout << "new connection: " << conn->peerAddress().toIpPort() << endl;
    }else{
        cout << "connection close: " << conn->peerAddress().toIpPort() << endl;
        conn->shutdown(); 
    }
}

void ChatServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    string msg = buf->retrieveAllAsString();
    cout << "recv data: " << msg << endl;
    json js = json::parse(msg); // 反序列化 

    auto msgHandler = ChatService::getInstance()->getHandler(js["msgid"].get<int>()); 
    // cout << "getHandler success" << endl;
    msgHandler(conn, js, time); 


}