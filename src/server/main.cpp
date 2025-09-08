#include "chatserver.hpp"
#include "chatservice.hpp" 
#include <iostream>
#include <signal.h>
using namespace std;

//处理服务器ctrl+c结束后 重置user的状态 
void resetHandler(int){
    ChatService::getInstance()->reset(); 
    exit(0); 
}


int main()
{
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();  //IO复用+ 线程池

    return 0;
}