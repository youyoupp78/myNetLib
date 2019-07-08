#pragma once
#include "Socket.h"
#include  <string>
#include <memory>
#
class Socket;
class TcpClient
{
public:
	TcpClient(std::string server, int port):pClient( new Socket(server,port))
	{
		  
	}
	~TcpClient() {}

	void Connect();
    
private:
	 
	std::shared_ptr<Socket> pClient;
};

