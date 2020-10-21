#include "stdafx.h"
#include "AdobeRegisterWindow.h"

AdobeRegisterWindow::AdobeRegisterWindow(CMarkObject* markObj)
{
	m_markObject = markObj;
	m_TimeoutCount = 0;
}

AdobeRegisterWindow::~AdobeRegisterWindow()
{
	m_markObject = NULL; 
	m_TimeoutCount = 0;
}

void AdobeRegisterWindow::RegisterWindowHandler(Function_T function, int waittime)
{
	CloseAsync();
	m_markObject->SleepAndRecord(2000);
	if (function() != 0)
		throw;
	m_markObject->SleepAndRecord(waittime * 1000);
	WaitClose();
}

int AdobeRegisterWindow::CloseAsync()
{
	m_bExitThread = false;
	m_threadClose.swap(std::thread([](AdobeRegisterWindow* pWindow)->void {

		pWindow->ThreadLoopClose();
	}, this));

	return 0;
}

int AdobeRegisterWindow::WaitClose()
{
	m_TimeoutCount = INT64_MAX;
	m_bExitThread = true;
	if (m_threadClose.joinable())
	{
		m_threadClose.join();
	}

	return 0;
}

int AdobeRegisterWindow::Close()
{
	HWND hWndParent = NULL, hRegister = NULL;
		
	if (NULL == (hRegister = FindWindow(_T("EmbeddedWB"), _T(""))))
	{
		return -1;
	}
	 
	MyRegisterClass();
	hWndParent = CreateWindow(WINDOW_CLASS_NAME, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, GetModuleHandle(NULL), nullptr);
	DWORD dwError = GetLastError();
	if (NULL == hWndParent)
	{
		return -2;
	}

	SetParent(hRegister, hWndParent);
	::ShowWindow(hRegister, SW_HIDE);
	::EndDialog(hRegister, 0);
	::DestroyWindow(hWndParent);

	return 0;
}

int AdobeRegisterWindow::ThreadLoopClose()
{
	while (!m_bExitThread)
	{
		 Sleep(10);
		if (0 != Close())
		{
			m_TimeoutCount += 10;
			if (m_TimeoutCount >= INT64_MAX)
				break;
			else
				continue;
		}

		break;
	}

	return 0;
}

int AdobeRegisterWindow::MyRegisterClass()
{
	WNDCLASS wcex = { 0 };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	return RegisterClass(&wcex);
}
