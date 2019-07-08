#pragma once
/*封装socket API的操作*/
#include <string>
#include <map>
#include "netheader.h"
#include <memory>
#include "EventPoll.h"
#define INITSOCKET -1
#define MAXSLEEP   10



class Socket
{
public:
	Socket(std::string peer,int p,std::string host = "localhost"):
		       sfd(INITSOCKET),port(p),hostName(host),peerName(peer)
	{
		initSocket();
	}

	Socket(int p, std::string host = "localhost"):hostName(host),port(p)
	{
		initSocket();
	}
	~Socket();

public:
	bool initSocket();
	bool getHostIp(std::string hostName);
	bool onBind();
	bool onListen();
	void onConnect();
	bool onClose();
	int  con_retry();
	int  getSocketfd()
	{
		return sfd;
	}
	int  getClientfd()
	{
		return clientfd;
	}
	int getClientPort()
	{
		return clientaddr.sin_port;
	}

	void onAccept();
	/*
	static void onAccept(void *arg);
	static void onRecv(void *arg);
	static void onSend(void *arg);
*/
	
private:
	int  sfd;//TODO每个端口一个fd
	int  port;//TODO多个端口 监听
	int  clientfd;
	std::string hostName;
	std::string peerName;
	std::map<std::string,int> HostInfo;
	struct sockaddr_in clientaddr;

public:
	std::map<int,int> clientMap;
	//std::shared_ptr<EventPoll> ptrEpoll;



};

