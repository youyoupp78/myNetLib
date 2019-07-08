#include "TcpServer.h"


TcpServer::TcpServer(int port):pSocket(new Socket(port)), pEpoll(new EventPoll(MAXEVENTS))
{
	pSocket->onListen();
	RegEvent();
}


void TcpServer::Accept(void * arg)
{
	TcpServer *pTcpSer = (TcpServer *)arg;
	pTcpSer->pSocket->onAccept();
}

void TcpServer::RegEvent()
{
	Event ev ;
	ev.fd = pSocket->getSocketfd();
	ev.arg = NULL;
	ev.event = EPOLLIN;
	ev.func = Accept;
	pEpoll->addEpollfd(ev);


	
}

void TcpServer::Loop()
{
     while(1)
	 {
		 pEpoll->waitEvent();
	 }
}