#pragma once

#include "SettingObject.h"

class OutlookSetting : public SettingObject
{
	
public:
	OutlookSetting();
	~OutlookSetting();

	
	int Set();

private:
	int Init();
	int Destroy();
	//int StartAppThread();
private:
	std::thread m_threadDisplay;
	//Outlook::_ApplicationPtr m_pApp;
	//TCHAR m_szOutlookFP[_MAX_PATH] = { 0 };
};