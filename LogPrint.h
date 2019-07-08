#pragma once
#include <string>
#define LOG_PRINT(fmt,args...)  {printf(fmt,##args);}

#define WRITE_LOG(buf,str) \
        int ibuflen =sizeof(buf) \
        snprintf(buf,sizeof(buf),"(%s,%d):%s",__FILE__,__LINE__,buf);\
        printf("%s\n",buff);

class LogPrint
{
public:
	LogPrint();
	~LogPrint();

};

