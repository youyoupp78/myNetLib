#pragma once
#include <string>

class ThreadBase
{
public:
	ThreadBase() {}
	ThreadBase(std::string & threadName):taskName(threadName),pTaskData(NULL)
	{

	}
	virtual void run() =0;

	void setTaskData(void * data) 
	{
		pTaskData = data;
	}
protected:
    std::string taskName;
	void * pTaskData;
};

