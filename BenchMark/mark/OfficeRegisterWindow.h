#pragma once
#include "MarkObject.h"

class OfficeRegisterWindow
{
	const TCHAR* WINDOW_CLASS_NAME = _T("CloseOfficeRegisterWindow");
	const TCHAR* WINDOW_TITLE = _T("Close");
 public:
	OfficeRegisterWindow();
	OfficeRegisterWindow(CMarkObject* markObj);
	~OfficeRegisterWindow();

	int CloseAsync();
	int WaitClose();

	int CloseAcceptWindow();
	int CloseRegisterWindow();
	void RegisterWindowHandler(Function_T function);
private:
	int ThreadCloseRegisterWindow();
	
	int MyRegisterClass();
	

	bool		m_bExitThread = true;
	std::thread m_threadCloseRegisterWnd;
	CMarkObject*	m_markObject;
	void SleepAndRecord(ULONGLONG);
	ULONGLONG	m_TimeoutCount;
};


