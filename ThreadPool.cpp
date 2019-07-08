#include "ThreadPool.h"
#include "LogPrint.h"
#include "pthread.h"

std::vector<ThreadBase *>  ThreadPool:: arrThreadTask;
bool ThreadPool::shutdown = false;
int ThreadPool::threadNum = 0;

pthread_mutex_t ThreadPool::pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  ThreadPool::pthread_cond = PTHREAD_COND_INITIALIZER;

ThreadPool::ThreadPool(int nums)
{
	threadNum = nums;
	int  ret = createThread();
}


ThreadPool::~ThreadPool()
{

}

void * ThreadPool::threadFun(void * threadData)
{
	pthread_t tid = pthread_self();
	while( true )
	{
		pthread_mutex_lock( &pthread_mutex);
		while( arrThreadTask.size() == 0 && !shutdown )
		{
			//空线程阻塞中
			pthread_cond_wait(&pthread_cond, &pthread_mutex);
			if(shutdown)
			{
				pthread_mutex_unlock(&pthread_mutex);
				//线程退出
				pthread_exit(NULL);//无需相关销毁资源NULL
			}
			 
			std::vector<ThreadBase *>::iterator it = arrThreadTask.begin();
			ThreadBase * task = *it;
			if( it != arrThreadTask.end())
			{
				task = * it;
				arrThreadTask.erase(it);
			}

			pthread_mutex_unlock(&pthread_mutex);
			task->run();
		}
		return ((void * ) 0);

	}
}

int ThreadPool:: addTask(ThreadBase * task)
{
	pthread_mutex_lock(&pthread_mutex);
	arrThreadTask.push_back(task);
	pthread_mutex_unlock(&pthread_mutex);
	pthread_cond_signal(&pthread_cond);
	return 0;
}

int ThreadPool::createThread()
{
	ptid = new pthread_t[threadNum];
	for( int i = 0; i <threadNum ; i++)
	{
		pthread_create(&ptid[i],NULL, threadFun, NULL);
	}
	return 0;
}

int ThreadPool::stopAll()
{
	if(shutdown)
	{
		return -1;
	}
	pthread_cond_broadcast(&pthread_cond);
	for( int i = 0; i < threadNum; i++)
	{
		 //回收资源
		pthread_join(ptid[i],NULL);
	}
	delete [] ptid;
	ptid = NULL;
	pthread_mutex_destroy(&pthread_mutex);
	pthread_cond_destroy(&pthread_cond);
    return 0;	
}

int ThreadPool::getTaskSize()
{
	return (int) arrThreadTask.size();
}
