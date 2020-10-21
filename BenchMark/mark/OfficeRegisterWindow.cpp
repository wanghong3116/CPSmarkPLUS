#include "stdafx.h"
#include "OfficeRegisterWindow.h"

OfficeRegisterWindow::OfficeRegisterWindow()
{
	m_markObject = NULL;
	m_TimeoutCount = 0;
}

OfficeRegisterWindow::OfficeRegisterWindow(CMarkObject* markObj)
{
	m_markObject = markObj;
	m_TimeoutCount = 0;

}

void OfficeRegisterWindow::RegisterWindowHandler(Function_T function)
{
	CloseAsync();
	SleepAndRecord(2000);
	if (function() != 0)
		throw;
	SleepAndRecord(5000);
	WaitClose();
}

OfficeRegisterWindow::~OfficeRegisterWindow()
{
}

void OfficeRegisterWindow::SleepAndRecord(ULONGLONG time)
{
	if (m_markObject != NULL)
		m_markObject->SleepAndRecord(time);
	else
		Sleep((DWORD)time);
}

int OfficeRegisterWindow::CloseAsync()
{
	m_bExitThread = false;

	m_threadCloseRegisterWnd.swap(std::thread([](OfficeRegisterWindow* pORWindow)->void {

		pORWindow->ThreadCloseRegisterWindow();

		return;
	}, this));

	return 0;
}

int OfficeRegisterWindow::CloseAcceptWindow()
{
	HWND hAcceptWnd = NULL;
	RECT rectWindow = { 0 };
	POINT pointClick = { 0 };

	if (NULL == (hAcceptWnd = ::FindWindow(_T("NUIDialog"), _T("首要事项"))))
	{
	 	return -1;
	}
	SetWindowPos(hAcceptWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	GetWindowRect(hAcceptWnd, &rectWindow);
	pointClick.x = rectWindow.right - 45;
	pointClick.y = rectWindow.bottom - 35;
	SetCursorPos(pointClick.x, pointClick.y);
	Sleep(1000);
	doLButton(pointClick.x, pointClick.y);
	Sleep(1000);

	return 0;
}

int OfficeRegisterWindow::CloseRegisterWindow()
{
	HWND hWndParent = NULL, hRegister = NULL;

	MyRegisterClass();
	hWndParent = CreateWindow(WINDOW_CLASS_NAME, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, GetModuleHandle(NULL), nullptr);
	DWORD dwError = GetLastError();
	if (NULL == hWndParent)
	{
		return -1;
	}

	hRegister = FindWindow(_T("NUIDialog"), _T("输入您的产品密钥"));
	if (NULL == hRegister)
	{
		hRegister = FindWindow(_T("NUIDialog"), _T("输入产品密钥"));
		if (NULL == hRegister)
		{
			hRegister = FindWindow(_T("NUIDialog"), _T("帐户"));
			if (NULL == hRegister)
			{
				hRegister = FindWindow(_T("NUIDialog"), _T(""));
			}
		}
	}
	
	//HWND hSrcParent = ::GetParent(hRegister);
	//::DestroyWindow(hRegister);
	//::EndDialog(hRegister, 0);

	SetParent(hRegister, hWndParent);

	::ShowWindow(hRegister, SW_SHOW);
	::EndDialog(hRegister, 0);
	//::SendMessage(hRegister, WM_CLOSE, 0, 0);
	::DestroyWindow(hWndParent);
	return 0;
}

int OfficeRegisterWindow::WaitClose()
{
	m_TimeoutCount = INT64_MAX;
	m_bExitThread = true;
	if (m_threadCloseRegisterWnd.joinable())
	{
		m_threadCloseRegisterWnd.join();
	}

	return 0;
}

int OfficeRegisterWindow::ThreadCloseRegisterWindow()
{
	while (!m_bExitThread)
	{
		CloseAcceptWindow();
		CloseRegisterWindow();
		Sleep(10);
		m_TimeoutCount += 10;
		if (m_TimeoutCount >= INT64_MAX)
		{
			if (m_markObject != NULL)
				m_markObject->LogError({ _T("ThreadCloseRegisterWindow: Wait time out.") });
			break;
		}
	}
	return 0;
}

int OfficeRegisterWindow::MyRegisterClass()
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
