#include "Socket.h"
#include "LogPrint.h"

#define  MAXLISTENFD 10

bool Socket::initSocket()
{
	if((sfd =socket(AF_INET,SOCK_STREAM,0))< 0)
    return false;
	return onBind();

}

Socket::~Socket()
{
	close(sfd);
}

bool Socket::onListen()
{
	return ( -1 == listen(sfd,MAXLISTENFD))?false:true;
}

/*获取服务IP TODO*/

bool Socket::getHostIp( std::string hostName)
{
	hostent  *ht = gethostbyname(hostName.c_str());
	if (NULL == ht)
	{
		LOG_PRINT("get hostNames error\n");
		return false;
	}
	if (NULL == ht->h_addr_list)
	{
		LOG_PRINT("get hostNames error\n");
		return false;
	}
	for (int i = 0; ht->h_addr_list[i] != NULL; i++)
	{
		char *host = (inet_ntoa(*(struct in_addr *)ht->h_addr_list[i]));
		LOG_PRINT("host>> %s", host);
		// TODO 保存多个IP
		HostInfo[host] = port;
	}
	//抛异常TODO
	return true;
}
void Socket::onConnect()
{
	 
	if( con_retry() <0 )
	{
		LOG_PRINT("connect error\n");
	}
}

int Socket::con_retry()
{
	if(!getHostIp(peerName)) return -1;
	int nsec = 1;
	struct sockaddr_in serveraddr,clientaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = port;
	//TODO暂时取第一个 获取服务的IP
	auto it = HostInfo.begin();
	serveraddr.sin_addr.s_addr =((struct in_addr *)&(*it))->s_addr;
	for (; nsec <= MAXSLEEP; nsec <<= 1)
	{
		/*连接成功返回*/
		int cfd = connect(sfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
		if (cfd == 0)
		{
			int len = sizeof(clientaddr);
			int cl = getsockname(sfd, (struct sockaddr *)&clientaddr, (socklen_t *)(&len));
			int port = clientaddr.sin_port;
			clientfd = cfd;
			return  cfd;
		}
		/*sleep后重试*/
		else if (nsec <= MAXSLEEP / 2)
		{
			LOG_PRINT("wait a sleep\n");
			sleep(nsec);
		}

	}
	return -1;

}

bool Socket::onBind()
{
	//绑定的server
	if( !getHostIp(hostName)) return false;
    struct sockaddr_in  hostaddr;
	auto it = HostInfo.begin();
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(port);
	hostaddr.sin_addr.s_addr  = ((struct in_addr* ) &(it->first))->s_addr;
	if (bind(sfd,(struct sockaddr *)&hostaddr,sizeof(hostaddr)) < 0)
	{
		LOG_PRINT("bind error :%s\n");
		return false;
	}
	return true;
}

bool Socket::onClose()
{
	if(sfd  == INITSOCKET)
	{
		close(sfd);
	}
}

void Socket::onAccept()
{
	int len = sizeof(clientaddr);
	int accfd = accept(sfd, (struct sockaddr *)&clientaddr, (socklen_t*)(&len));
	if (accfd < 0)
	{
		LOG_PRINT("connect fail~~\n");
	}
	else
	{
		LOG_PRINT("connect succ~~\n");
		clientMap[accfd] = clientaddr.sin_port;
	}
}

