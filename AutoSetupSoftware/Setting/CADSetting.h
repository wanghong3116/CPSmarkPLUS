#pragma once

#include "AutoDeskSetting.h"

class CADSetting : public AutoDeskSetting
{
public:
	CADSetting();
	~CADSetting();

	
	virtual int Set();
	

private:
	virtual int Init();



	virtual int Destroy();
	
};