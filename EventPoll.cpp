#include "EventPoll.h"
#include "unistd.h"
/*
EPOLLIN ����ʾ��Ӧ���ļ����������Զ��������Զ�SOCKET�����رգ���
EPOLLOUT����ʾ��Ӧ���ļ�����������д��
EPOLLPRI����ʾ��Ӧ���ļ��������н��������ݿɶ�������Ӧ�ñ�ʾ�д������ݵ�������
EPOLLERR����ʾ��Ӧ���ļ���������������
EPOLLHUP����ʾ��Ӧ���ļ����������Ҷϣ�
EPOLLET�� ��EPOLL��Ϊ��Ե����(Edge Triggered)ģʽ�����������ˮƽ����(Level Triggered)��˵�ġ�
EPOLLONESHOT��ֻ����һ���¼���������������¼�֮���������Ҫ�����������socket�Ļ�����Ҫ�ٴΰ����socket���뵽EPOLL������
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

