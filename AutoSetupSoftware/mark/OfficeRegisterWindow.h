#pragma once

class OfficeRegisterWindow
{
	const TCHAR* WINDOW_CLASS_NAME = _T("CloseOfficeRegisterWindow");
	const TCHAR* WINDOW_TITLE = _T("Close");
public:
	OfficeRegisterWindow();
	~OfficeRegisterWindow();

	int CloseAsync();
	int WaitClose();

	int CloseAcceptWindow();
	int CloseRegisterWindow();
private:
	int ThreadCloseRegisterWindow();
	
	int MyRegisterClass();
	

	bool		m_bExitThread = true;
	std::thread m_threadCloseRegisterWnd;

};


