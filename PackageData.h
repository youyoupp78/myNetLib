#pragma once
class PackageData
{
public:
	PackageData();
	~PackageData();
    
	//���ݴ���
	virtual void handlerData() = 0 ;

	virtual  long getDataSize() = 0;
};



