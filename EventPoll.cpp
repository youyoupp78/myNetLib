#include "EventPoll.h"
#include "unistd.h"
/*
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
*/
#include "LogPrint.h"




EventPoll::~EventPoll()
{
	close(epoll_fd);
}

void EventPoll::createEpoll( )
{
	 epoll_fd = epoll_create(MAXEVENTS);
}

int EventPoll::addEpollfd(const Event &event)
{
	struct epoll_event ev;
	ev.data.fd = event.fd;
	ev.events = EPOLLIN;
	/*std::shared_ptr<Event> pEv(new Event);
	pEv->data.fd = fd;
	pEv->events = EPOLLIN;*/
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD,event.fd,&ev)<0)
	{
		LOG_PRINT(" op add fd fail\n");
		return -1;
	}
	mapEvents[event.fd] = event;
	return 0;
}

int EventPoll::delEpollfd( const Event &event)
{
	struct epoll_event ev;
	ev.data.fd = event.fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.fd, &ev) < 0)
	{
		LOG_PRINT(" op del fd fail\n");
		return -1;
	}
	mapEvents.erase(event.fd);
	return 0;
}

int EventPoll::waitEvent( )
{
	struct epoll_event EventPoll[MAXEVENTS];
	int nready = epoll_wait(epoll_fd, EventPoll,MAXEVENTS,500);
	if(nready < 0)
	{
		LOG_PRINT("epoll wait error");
		return -1;
	}
	for(int i = 0; i < nready; i++)
	{
		  int fd = EventPoll[i].data.fd;
		  Event event = mapEvents[fd];
		  if(event.func)
		  {
			  event.func(event.arg);
		  }

	}
	return 0;
}

