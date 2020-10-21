#pragma once


class SettingObject
{
protected:
	SettingObject();
public:
	SettingObject(const TCHAR* szMainClassName, const TCHAR* szMainWindowName, const TCHAR* szStartArgs);
	~SettingObject();

	
	virtual int Set() = 0;
	

protected:
	virtual int Init() = 0;
	int StartApp();

	virtual int Destroy() = 0;

	bool IsFindMainWindowAndVisible();
protected:
	PROCESS_INFORMATION m_pi = { 0 };
	
	TCHAR				m_szExeFP[_MAX_PATH] = { 0 };

	TCHAR				m_szMainClassName[_MAX_PATH] = { 0 };
	TCHAR				m_szMainWindowName[_MAX_PATH] = { 0 };
	TCHAR				m_szStartArgs[_MAX_PATH] = { 0 };
};