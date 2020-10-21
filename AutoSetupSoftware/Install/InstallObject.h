#pragma once


class InstallObject
{
	InstallObject();
public:
	InstallObject(const TCHAR* szSetupFP);
	~InstallObject();

	virtual int Install();
	virtual int WaitInstallOver();
protected:
	int runProcess(TCHAR *szCmd, const TCHAR* szRunDir, HANDLE& hProcess, HANDLE& hThread);

protected:
	TCHAR	m_szSetupFilePath[_MAX_PATH];

	HANDLE	m_hProcess	= NULL;
	HANDLE	m_hThread	= NULL;
};