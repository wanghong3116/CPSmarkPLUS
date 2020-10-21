#pragma once

#include "SettingObject.h"

class AutoDeskSetting : public SettingObject
{
	AutoDeskSetting();
public:
	AutoDeskSetting(const TCHAR* szMainClassName, const TCHAR* szMainWindowName, const TCHAR* szStartArgs);
	~AutoDeskSetting();

	
	virtual int Set() = 0;
	

protected:
	virtual int Init() = 0;

	int ClickSerialNumber();

	int ClickPermit();

	int ClickRun();

	int ClickSelectStyle();

	int ClickDataCollect();

	virtual int Destroy() = 0;

protected:
// 	PROCESS_INFORMATION m_pi = { 0 };
// 	
// 	TCHAR				m_szExeFP[_MAX_PATH] = { 0 };
// 
// 	TCHAR				m_szMainClassName[_MAX_PATH] = { 0 };
// 	TCHAR				m_szMainWindowName[_MAX_PATH] = { 0 };
// 	TCHAR				m_szStartArgs[_MAX_PATH] = { 0 };
};