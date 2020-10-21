#pragma once
#include "MarkObject.h"

class AdobeRegisterWindow
{
	const TCHAR* WINDOW_CLASS_NAME = _T("CloseRegisterWindow");
	const TCHAR* WINDOW_TITLE = _T("Close");

public:
	AdobeRegisterWindow(CMarkObject* markObj);
	~AdobeRegisterWindow();

	int CloseAsync();
	int Close();
	int WaitClose();
	void RegisterWindowHandler(Function_T function, int waittime = 10);
private:
	int ThreadLoopClose();
	
	int MyRegisterClass();
	

	bool		m_bExitThread = true;
	std::thread m_threadClose;
	CMarkObject*	m_markObject;
	ULONGLONG		m_TimeoutCount;
};


