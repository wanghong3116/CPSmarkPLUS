#pragma once

#include "AutoDeskSetting.h"

class DMaxSetting : public AutoDeskSetting
{
	
public:
	DMaxSetting();
	~DMaxSetting();

	
	virtual int Set();
	

private:
	virtual int Init();

	//int ClickSerialNumber();

	//int ClickPermit();

	//int ClickRun();

	//int ClickSelectStyle();

	//int ClickDataCollect();

	virtual int Destroy();
	
};