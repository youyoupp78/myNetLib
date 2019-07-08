#pragma once
class PackageData
{
public:
	PackageData();
	~PackageData();
    
	//数据处理
	virtual void handlerData() = 0 ;

	virtual  long getDataSize() = 0;
};



