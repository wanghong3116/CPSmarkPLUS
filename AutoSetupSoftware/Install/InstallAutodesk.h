#pragma once

#include "InstallObject.h"

class InstallAutodesk : public InstallObject
{
	InstallAutodesk();
public:
	InstallAutodesk(const TCHAR* szSetupFP, const TCHAR* szSetupWindowName);
	~InstallAutodesk();

	virtual int Install();

protected:
	TCHAR m_szSetUpWindowName[_MAX_PATH] = { 0 };
private:
	HWND FindSetupWindow(int nFindMaxTime);

	int StartPage();
	int AcceptPage();
	int SetupPage();
	int OverPage();

	HWND m_hSetupWindow = NULL;

	
};