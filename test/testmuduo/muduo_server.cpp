

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Timestamp.h>
#include <iostream>
#include <string>
using namespace std;
using namespace muduo::net; 
using namespace muduo;  // 添加muduo命名空间以使用Timestamp
using namespace placeholders;

// 成员有那些
// 构造需要什么参数
// 注册回调函数
// 设置线程数量
class ChatServer{
public:
    ChatServer(EventLoop *loop,
        const InetAddress &listenAddr,
        const string &nameArg)
        : _server(loop, listenAddr,nameArg), _loop(loop)
        {   //需要一个函数对象 ConnectionCallback 需要输入一个参数TcpConnectionPtr
            //我们需要这个函数可以访问类内变量 定一个类内函数--绑定this 保留_1 连接指针
            // 给服务器注册用户连接的创建和断开的回调
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));            
            // 给服务器注册用户读写事件回调
            _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));
            
            //线程
            _server.setThreadNum(4); 
        }
    void start(){
        _server.start();
    }

private:
    // 连接创建和断开
    void onConnection(const TcpConnectionPtr &conn)
    {

        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << " - "
                 << conn->localAddress().toIpPort() <<" status online "<< endl;
        }else{
            cout << conn->peerAddress().toIpPort() << " - "
                 << conn->localAddress().toIpPort() <<" status offline "<< endl;
           conn->shutdown();
        //    _loop->quit();
        }

    } 
// 处理读写事件
    void onMessage(const TcpConnectionPtr& conn,
                Buffer *buffer,
                Timestamp time){
        string buf = buffer->retrieveAllAsString();
        cout<<" recv data: "<<buf<<" time: "<<time.toString();/*  */

    }

    TcpServer _server;   //
    EventLoop *_loop; // epoll
} ;

int main(){
    EventLoop loop; 

    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"chatServer");

    server.start();
    loop.loop(); 

    return 0;

}