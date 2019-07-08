#pragma once
#include "ThreadBase.h"
#include <vector>
#include <pthread.h>

class ThreadBase;

class ThreadPool
{
public:
	ThreadPool( int nums);
	~ThreadPool();

public:
	int createThread();
	int addTask(ThreadBase * task);
	int stopAll();
	int getTaskSize();

	//线程的回调函数
	static void * threadFun(void * threadData);
	//线程结束
	static  int moveThread(pthread_t tid);
	//从列表中启用线程
	static  int useThread(pthread_t tid);

private:
	//多个线程类
	static std::vector<ThreadBase *>  arrThreadTask;
	static bool shutdown;
	static int  threadNum;
	pthread_t * ptid;

	//线程同步互斥量
	static pthread_mutex_t  pthread_mutex;
	//线程同步条件变量
	static pthread_cond_t   pthread_cond;


};

