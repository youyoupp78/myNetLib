#pragma once
#include <memory>
#include "Socket.h"
#include "EventPoll.h"
class TcpServer 
{
public:
	TcpServer(int port);
	~TcpServer() {}
	void Loop();
	void RegEvent();
	static void Accept(void * arg);
	static void RecvData(void * arg);
public:
	struct epoll_event ev;
	std::shared_ptr<Socket> pSocket;
	std::shared_ptr<EventPoll> pEpoll;

};

