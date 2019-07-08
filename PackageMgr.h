#pragma once
#include "PackageData.h"
#include <string>

struct packageheader
{
    long datasize ;
	int  serialno ;
	char Id[256]; 
};


class PackageMgr
{
public:
	PackageMgr();
	~PackageMgr();
public:
	struct packagheader * phead;
	PackageData * pdata;
};

