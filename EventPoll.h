#pragma once
#include <sys/epoll.h>
#include <memory>
#include <map>
#define MAXEVENTS   20

//»Øµ÷º¯Êý
typedef void(*cb_fun)(void * arg);
struct Event
{
	int fd;
	short event;
	void *arg;
	cb_fun func;
};


class CEventBase
{
public:
	int fd;
	short event;
	void * arg;
public:
	void operator()(cb_fun func,void * arg =NULL)
	{
		 
	}
};

class EventPoll
{
public:
	EventPoll(int nums) :eventsize(nums) {}
	~EventPoll();

public:

	void createEpoll();
	int  addEpollfd(const Event &event);
	int  delEpollfd(const Event &event);
	int  waitEvent();


private:
	int epoll_fd;
	int eventsize;
	//std::map<int, std::shared_ptr<Event>>  mapEvents;
	std::map<int,Event> mapEvents;
};

