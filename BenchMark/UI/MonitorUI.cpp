// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "MonitorUI.h"
#include "MarkObject.h"
#include "PremiereMark.h"
#include "Photoshop.h"
#include "ChromeMark.h"
#include "OfficeWordMark.h"
#include "OfficeExcelMark.h"
#include "OfficePowerPointMark.h"
#include "ZipMark.h"
#include "VideoPlayer.h"
#include "AEMark.h"
#include "HandBrake.h"
#include "OpenCL.h"
#include "3DSMax.h"
#include "Resource.h"
#include "WinrarZipMark.h"
#include "PDFMark.h"
#include "3DSMaxWhale.h"
#include "3DSMaxCar.h"
#include "3DSMaxHome.h"
#include "ResultDetailUI.h"







CMonitorUI::CMonitorUI(HWND hParent) :
	m_hParent(hParent)
{

}

CMonitorUI::~CMonitorUI()
{

}

void CMonitorUI::LoadSkin()
{
	

}

void CMonitorUI::Notify(DuiLib::TNotifyUI & msg)
{
	DuiLib::CDuiString strType = msg.sType;

	if (strType == _T("click"))
	{
		OnNotifyBnClick(msg);
	}
	else if (strType == _T("setfocus"))
	{
	}
	else if (strType == _T("windowinit"))
	{
		OnNotifyWindowInit();
	}
}

LRESULT CMonitorUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMonitorUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CMonitorUI::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMonitorUI::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMonitorUI::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMonitorUI::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (NULL != m_pverticalResult && m_pverticalMonitor->IsVisible())
	{
		return HTCAPTION;
	}

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (pt.y < 36 && pt.x < 1100)
	{
		return HTCAPTION;
	}
	
	return HTCLIENT;

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_pm.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CMonitorUI::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		RECT rc = { rcClient.left, rcClient.top + szRoundCorner.cx, rcClient.right, rcClient.bottom };
		HRGN hRgn1 = ::CreateRectRgnIndirect(&rc);
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		::CombineRgn(hRgn1, hRgn1, hRgn2, RGN_OR);
		::SetWindowRgn(*this, hRgn1, TRUE);
		::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CMonitorUI::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	//rcWork.Offset(-rcWork.left, -rcWork.top);
	rcWork.OffsetRect(-rcWork.left, -rcWork.top);
	
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CMonitorUI::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CMonitorUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:			lRes = OnCreate(bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_TIMER:			lRes = OnTimer((UINT)wParam, bHandled); break;
	case WS_CHANGE_RUNNING_INFOR: lRes = OnChangeRunningInfor((const TCHAR*)lParam, bHandled); break;
	case WS_MARK_WORK_OVER: lRes = OnMarkWorkOver(bHandled); break;
	case WS_MARK_WORK_PROGRESS: lRes = OnMarkWorkProgress(wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

int CMonitorUI::DoModal()
{
	Create(m_hParent, NULL, UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE | WS_EX_APPWINDOW);
	ShowWindow(true);
	
	SetIcon(IDR_MAINFRAME);
	DuiLib::CPaintManagerUI::MessageLoop();

	return TRUE;
}

LRESULT CMonitorUI::OnCreate(BOOL& bHandled)
{
	bHandled = FALSE;

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	m_pRoot = builder.Create(_T("Monitor.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(m_pRoot && "Failed to parse XML");
	m_pm.AttachDialog(m_pRoot);
	m_pm.AddNotifier(this);

	/*pRoot->GetWidth()*/
	

	//m_pWndShadow = new DuiLib::CWndShadow;
	//m_pWndShadow->Create(m_hWnd);
	//RECT rcCorner = { 3,3,4,4 };
	//RECT rcHoleOffset = { 0,0,0,0 };
	//m_pWndShadow->SetImage(_T("LeftWithFill.png"), rcCorner, rcHoleOffset);

	//DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
	//SetWindowAttribute(m_hWnd, DWMWA_TRANSITIONS_FORCEDISABLED, &ncrp, sizeof(ncrp));
	return TRUE;
}


void CMonitorUI::OnNotifyBnClick(DuiLib::TNotifyUI & msg)
{
	DuiLib::CDuiString strName = msg.pSender->GetName();

	if (strName == _T("buttonAnalizeClose"))
	{
		Close(0);
	}
	else if (strName == _T("buttonTotalLevel"))
	{
		OnBnClickTotalLevel();
	}
}


void CMonitorUI::OnNotifyWindowInit()
{
	m_pverticalMonitor = (DuiLib::CVerticalLayoutUI*)m_pm.FindControl(_T("verticalMonitor"));
	//m_pverticalResult = (DuiLib::CHorizontalLayoutUI*)m_pm.FindControl(_T("verticalResult"));
	m_pverticalResult = (DuiLib::CHorizontalLayoutUI*)m_pm.FindControl(_T("verticalAnalyze"));
	m_ptextCPUUtilization = (DuiLib::CTextUI*)m_pm.FindControl(_T("textCPUUtilization"));
	m_ptextCurrentWrokName = (DuiLib::CTextUI*)m_pm.FindControl(_T("textCurrentWrokName"));
	m_pprogressMarkWork = (DuiLib::CProgressUI *)m_pm.FindControl(_T("progressMarkWork"));

	SetTimer(this->GetHWND(), IDT_TIME_CPU_UTILIZATION, 500, NULL);
	{
		TCHAR szRunningInforFP[_MAX_PATH] = { 0 };
		SYSTEMTIME st = { 0 };

		MyPlatform::GetFullPathUnderExe(_T("RunningInformation"), szRunningInforFP);
		MyPlatform::CreateFullDirectory(szRunningInforFP);

		GetSystemTime(&st);
		_stprintf_s(szRunningInforFP + _tcslen(szRunningInforFP), _ARRAYSIZE(szRunningInforFP) - _tcslen(szRunningInforFP), _T("\\%d-%d-%d %d-%d-%d.txt"), 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		_tfopen_s(&m_pfileRunningInfor, szRunningInforFP, _T("wb"));
#ifdef UNICODE
		unsigned short usUnicodeHeader = 0;

		FILE_UNICODE_HEADER(usUnicodeHeader);
		fwrite(&usUnicodeHeader, sizeof(unsigned short), 1, m_pfileRunningInfor);
#endif
	}
	m_vMarkWorkRunInfor.clear();
	m_bExitThreadMark = FALSE;
	m_hThreadMark = CreateThread(NULL, 0, ThreadProcMarkWork, this, 0, NULL);

	{
		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		CRect rectClient = { 0 };

		GetClientRect(GetHWND(), rectClient);
		SetWindowPos(GetHWND(), HWND_TOPMOST, nScreenWidth - rectClient.Width(), 0, 0, 0, SWP_NOSIZE);
	}
	

}

LRESULT CMonitorUI::OnTimer(UINT nID, BOOL& bHandled)
{
	bHandled = TRUE;
	if (nID != IDT_TIME_CPU_UTILIZATION || NULL == m_ptextCPUUtilization)
	{
		bHandled = FALSE;
		return FALSE;
	}

	TCHAR szText[_MAX_PATH] = { 0 };
	
	_stprintf_s(szText, _ARRAYSIZE(szText), _T("CPU使用率%d%%"), m_cpuUsage.Get());
	m_ptextCPUUtilization->SetText(szText);
	SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

DWORD WINAPI CMonitorUI::ThreadProcMarkWork(_In_ LPVOID lpParameter)
{
	CMonitorUI* pThis = (CMonitorUI*)lpParameter;

	return pThis->MarkWork();
}

int __stdcall MarkStepInformationCallback(void* pParam, const TCHAR* szFormat, ...)
{
	va_list vl = NULL;
	TCHAR* szInformation = NULL;
	int nLen = 0;
	CMonitorUI* pMonitorUI = (CMonitorUI*)pParam;

	va_start(vl, szFormat);
	nLen = _vsctprintf(szFormat, vl);
	nLen += 200;
	if (NULL == (szInformation = (TCHAR*)calloc(nLen, sizeof(TCHAR))))
	{
		return -1;
	}
	_vstprintf_s(szInformation, nLen, szFormat, vl);

	pMonitorUI->SendMessage(WS_CHANGE_RUNNING_INFOR, 0, (LPARAM)szInformation);

	FREE(szInformation);

	va_end(vl);

	return TRUE;
}

DWORD CMonitorUI::MarkWork()
{
	std::vector<CMarkObject*> vpMarkObject;
	MarkWorkRunningInfor mwri = { 0 };
	int nMarkCount = 0;

	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_3DSMAX_CAR, (C3DSMaxCarMark*)NULL);
 	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_3DSMAX_HOME, (C3DSMaxHomeMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_3DSMAX_WHALE, (C3DSMaxWhaleMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_AE, (CAEMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_NET, (CChromeMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_HANDBRAKE, (CHandBrakeMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OFFICE_EXCEL, (COfficeExcelMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OFFICE_POWERPOINT, (COfficePowerPointMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OFFICE_WORD, (COfficeWordMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OPNCL, (COpenCLMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_PDF, (CPDFMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_PHOTOSHOP, (CPhotoshopMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_PREMIERE, (CPremiereMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_VIDEOPLAYER, (CVideoPlayerMark*)NULL);
  	NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_WINRAR, (CWinrarZipMark*)NULL);

	nMarkCount = vpMarkObject.size();
	SendMessage(WS_MARK_WORK_PROGRESS, 0, nMarkCount);
	for (int nMarkIdx = 0; nMarkIdx < nMarkCount && !m_bExitThreadMark; nMarkIdx++)
	{
		SendMessage(WS_MARK_WORK_PROGRESS, 1, nMarkIdx);
		if (NULL == vpMarkObject[nMarkIdx])
		{
			continue;
		}
		AddRunInformation(_T("执行内容：%s\r\n\r\n"), vpMarkObject[nMarkIdx]->getDescript());

		vpMarkObject[nMarkIdx]->setMarkStepInformationCallbackFuncAddress(MarkStepInformationCallback, this);
#define RUN_MARK
#ifdef RUN_MARK
		AddRunInformation(_T("%s 测试初始化中...\r\n"), vpMarkObject[nMarkIdx]->getDescript());
		if (BENCH_MARK_OK != vpMarkObject[nMarkIdx]->init())
 		{
 			AddRunInformation(_T("初始化程序失败...\r\n")\
 				_T("错误原因：%s")\
 				_T("--------------------------------------------\r\n\r\n"),
 				vpMarkObject[nMarkIdx]->getLastError());
 			SDELETE(vpMarkObject[nMarkIdx]);
 			continue;
 		}
		AddRunInformation(_T("%s 测试初始化成功....\r\n"), vpMarkObject[nMarkIdx]->getDescript());
		Sleep(1000);
		AddRunInformation(_T("测试程序执行中...\r\n"));
		if (BENCH_MARK_OK != vpMarkObject[nMarkIdx]->run())
		{
			AddRunInformation(_T("运行中出错,错误原因%s\r\n")\
				_T("--------------------------------------------\r\n\r\n"),
				vpMarkObject[nMarkIdx]->getLastError());
			SDELETE(vpMarkObject[nMarkIdx]);
			continue;
		}
		vpMarkObject[nMarkIdx]->destroy();
		mwri.nRunningTime = vpMarkObject[nMarkIdx]->getRunningTime();
#else
		mwri.nRunningTime = 272056;
#endif
		//Sleep(2 * 1000);

		AddRunInformation(_T("执行时间:%lld\r\n-------------------------------------------------------\r\n\r\n"), vpMarkObject[nMarkIdx]->getRunningTime());

		_tcscpy_s(mwri.szDescript, _ARRAYSIZE(mwri.szDescript), vpMarkObject[nMarkIdx]->getDescript());
		
		mwri.eMarkType		= vpMarkObject[nMarkIdx]->getMarkType();
		mwri.eMarkClassify	= vpMarkObject[nMarkIdx]->getMarkClassify();
		m_vMarkWorkRunInfor.push_back(mwri);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

		//SDELETE(vpMarkObject[nMarkIdx]);
	}

	for (int nMarkIdx = 0; nMarkIdx < nMarkCount; nMarkIdx++)
	{
		SDELETE(vpMarkObject[nMarkIdx]);
	}


	SendMessage(WS_MARK_WORK_PROGRESS, 2, 0);
	AddRunInformation(_T("所有测试项目已经全部执行完毕!!!!!\r\n"));
	Sleep(1000);

	PostMessage(WS_MARK_WORK_OVER);
	return BENCH_MARK_OK;
}

int CMonitorUI::AddRunInformation(TCHAR* szFormat, ...)
{
	va_list vl = NULL;
	TCHAR* szInformation = NULL;
	int nLen = 0;

	va_start(vl, szFormat);
	nLen = _vsctprintf(szFormat, vl);
	nLen += 200;
	if (NULL == (szInformation = (TCHAR*)calloc(nLen, sizeof(TCHAR))))
	{
		return -1;
	}
	_vstprintf_s(szInformation, nLen, szFormat, vl);

	if (NULL != m_pfileRunningInfor)
	{
		_ftprintf(m_pfileRunningInfor, _T("%s"), szInformation);
	}
	SendMessage(WS_CHANGE_RUNNING_INFOR, 0, (LPARAM)szInformation);

	FREE(szInformation);
	
	va_end(vl);
	return BENCH_MARK_OK;
}

LRESULT CMonitorUI::OnChangeRunningInfor(const TCHAR* szRunningInfor, BOOL& bHandled)
{
	bHandled = TRUE;
	m_ptextCurrentWrokName->SetText(szRunningInfor);

	return TRUE;
}

LRESULT CMonitorUI::OnMarkWorkOver(BOOL& bHandled)
{

	bHandled = TRUE;
	m_bExitThreadMark = TRUE;
	WaitForSingleObject(m_hThreadMark, INFINITE);
	CloseHandle(m_hThreadMark);
	KillTimer(this->GetHWND(), IDT_TIME_CPU_UTILIZATION);
	if (NULL != m_pfileRunningInfor)
	{
		fclose(m_pfileRunningInfor);
		m_pfileRunningInfor = NULL;
	}

	//SetWindowPos(GetHWND(), HWND_BOTTOM, 0, 0, 644, 948, SWP_NOMOVE);
	SetWindowPos(GetHWND(), HWND_BOTTOM, 0, 0, 1141, 744, SWP_NOMOVE);
	CenterWindow();
	SetWindowPos(GetHWND(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pverticalMonitor->SetVisible(false);
	m_pverticalResult->SetVisible(true);



	DisplayResult();
	DisplayComputerInformation();
	return TRUE;
}



LRESULT CMonitorUI::OnMarkWorkProgress(int nCode, int nValue, BOOL& bHandled)
{
	bHandled = TRUE;
	switch (nCode)
	{
	case 0:
	{
		m_pprogressMarkWork->SetMaxValue(nValue);
		m_pprogressMarkWork->SetMinValue(0);
		m_pprogressMarkWork->SetValue(0);
	}
	break;

	case 1:
	{
		m_pprogressMarkWork->SetValue(nValue);
	}
	break;

	case 2:
	{
		m_pprogressMarkWork->SetValue(m_pprogressMarkWork->GetMaxValue());
	}
	break;

	default:
		break;
	}

	return BENCH_MARK_OK;
}

extern std::string GetComputerCPUName();
extern DWORD GetComputerCPUDominantFrequency();
extern int GetComputerCPUNumber(DWORD& processorPackageCount, DWORD& processorCoreCount, DWORD& logicalProcessorCount);
extern std::string GetComputerCPUIdentifier();
extern double GetComputerMemorySize();
extern DWORD GetComputerAdapterCount();
extern int GetComputerAdapterDescription(std::vector<std::string>& rvstrAdapterDescription);



void CMonitorUI::OnBnClickTotalLevel()
{
	CResultDetailUI resultDetailUI(this->GetHWND(), &m_vMarkWorkRunInfor);

	resultDetailUI.DoModal();
}

int CMonitorUI::SetLabelTextByName(const TCHAR* szLabelName, const TCHAR* szText)
{
	DuiLib::CLabelUI* pLabel = NULL;

	pLabel = (DuiLib::CLabelUI*)m_pm.FindControl(szLabelName);
	if (NULL == pLabel)
	{
		return -1;
	}
	pLabel->SetText(szText);

	return BENCH_MARK_OK;
}

const DWORD g_pdwStandardTime[E_MARK_TYPE_END] = {
	276349,//E_MARK_3DSMAX_HOME,
	376349,//E_MARK_AE,
	232383,//E_MARK_NET,
	249292,//E_MARK_HANDBRAKE,
	199178,//E_MARK_OFFICE_EXCEL,
	318579,//E_MARK_OFFICE_POWERPOINT,
	460781,//E_MARK_OFFICE_WORD,
	135273,//E_MARK_OPNCL,
	135273,//E_MARK_PDF,
	272056,//E_MARK_PHOTOSHOP,
	257191,//E_MARK_PREMIERE,
	113691,//E_MARK_VIDEOPLAYER,
	257620,//E_MARK_WINRAR,
};

int CMonitorUI::DisplayResult()
{
	enum EResultControlName
	{
		E_RESULT_CONTROL_NAME_BEG,

		E_BUTTON_MEASURE_NAME = E_RESULT_CONTROL_NAME_BEG,
		E_BUTTON_STANDARD_NAME,
		E_PROGRESS_MEASURE_NAME,
		E_PROGRESS_STANDARD_NAME,

		E_RESULT_CONTROL_NAME_END,

	};

	DWORD pdwMarkClassifyTime[E_MARK_CLASSIFY_END + 1] = { 0 };
	DWORD pdwMarkStdTime[E_MARK_CLASSIFY_END + 1] = { 0 };
	EMarkClassify eMarkClassify = E_MARK_CLASSIFY_END;
	EMarkType eMarkType = E_MARK_TYPE_END;
	DuiLib::CProgressUI* pProgressMeasure = NULL, *pProgressStandard = NULL;
	int nMarkCount = 0;
	const TCHAR* pppszResultControlName[E_MARK_CLASSIFY_END + 1][E_RESULT_CONTROL_NAME_END] = {
		{ _T("buttonMediaMeasurePoint"), _T("buttonMediaStandardPoint"), _T("progressMediaMeasurePoint"), _T("progressMediaStandardPoint") },
		{ _T("buttonOfficeMeasurePoint"), _T("buttonOfficeStandardPoint"), _T("progressOfficeMeasurePoint"), _T("progressOfficeStandardPoint") },
		{ _T("buttonNetMeasurePoint"), _T("buttonNetStandardPoint"), _T("progressNetMeasurePoint"), _T("progressNetStandardPoint") },
		{ _T("buttonOpenclMeasurePoint"), _T("buttonOpenclStandardPoint"), _T("progressOpenclMeasurePoint"), _T("progressOpenclStandardPoint") },
		{ _T("buttonTotalMeasurePoint"), _T("buttonTotalStandardPoint"), _T("progressTotalMeasurePoint"), _T("progressTotalStandardPoint")},
	};

	nMarkCount = m_vMarkWorkRunInfor.size();
	for (int nIdx = 0; nIdx < nMarkCount; nIdx++)
	{
		eMarkClassify = m_vMarkWorkRunInfor[nIdx].eMarkClassify;
		eMarkType = m_vMarkWorkRunInfor[nIdx].eMarkType;

		pdwMarkStdTime[E_MARK_CLASSIFY_END] += g_pdwStandardTime[eMarkType];
		pdwMarkStdTime[eMarkClassify] += g_pdwStandardTime[eMarkType];

		pdwMarkClassifyTime[E_MARK_CLASSIFY_END] += (DWORD)m_vMarkWorkRunInfor[nIdx].nRunningTime;
		pdwMarkClassifyTime[eMarkClassify] += (DWORD)m_vMarkWorkRunInfor[nIdx].nRunningTime;
	}

	for (int nIdx = 0; nIdx <= E_MARK_CLASSIFY_END; nIdx++)
	{
		SetLabelTextByName(pppszResultControlName[nIdx][E_BUTTON_MEASURE_NAME], MyPlatform::Int64ToString(pdwMarkClassifyTime[nIdx] / 1000).c_str());
		SetLabelTextByName(pppszResultControlName[nIdx][E_BUTTON_STANDARD_NAME], MyPlatform::Int64ToString(pdwMarkStdTime[nIdx] / 1000).c_str());

		pProgressMeasure = (DuiLib::CProgressUI*)m_pm.FindControl(pppszResultControlName[nIdx][E_PROGRESS_MEASURE_NAME]);
		pProgressStandard = (DuiLib::CProgressUI*)m_pm.FindControl(pppszResultControlName[nIdx][E_PROGRESS_STANDARD_NAME]);
		if (NULL == pProgressMeasure || NULL == pProgressStandard)
		{
			continue;
		}

		int nMax = pProgressStandard->GetMaxValue();
		int nStd = nMax * 2 / 3;
		int nMeasure = 0;
		pProgressStandard->SetValue(nStd);
		double dScale = (double)pdwMarkClassifyTime[nIdx] / pdwMarkStdTime[nIdx];
		nMeasure = (int)(dScale * nStd);
		nMeasure = nMeasure < nMax ? nMeasure : nMax;
		pProgressMeasure->SetValue(nMeasure);
	}


	return BENCH_MARK_OK;
}

int CMonitorUI::DisplayComputerInformation()
{
	DWORD dwPhysicalProcessorNumber = 0;
	DWORD dwProcessorCoreNUmber = 0;
	DWORD dwProcessorLogicalNumber = 0;
	std::vector<std::string> vstrAdapterDesc;

	GetComputerAdapterDescription(vstrAdapterDesc);
	GetComputerCPUNumber(dwPhysicalProcessorNumber, dwProcessorCoreNUmber, dwProcessorLogicalNumber);
	SetLabelTextByName(_T("buttonCPUName"), MyPlatform::CharToTchar(("CPU名字：" + GetComputerCPUName()).c_str()).c_str());
	SetLabelTextByName(_T("buttonCPUCoreNum"), (_T("CPU内核数目：") + MyPlatform::Int64ToString(dwProcessorCoreNUmber)).c_str());
	SetLabelTextByName(_T("buttonCPULogicalNum"), (_T("CPU逻辑核数目：") + MyPlatform::Int64ToString(dwProcessorLogicalNumber)).c_str());
	SetLabelTextByName(_T("buttonAdapterName"), 
		MyPlatform::CharToTchar(("显卡名字：" + ((vstrAdapterDesc.size()) == 0 ? std::string("") : vstrAdapterDesc[0])).c_str()).c_str());

	return BENCH_MARK_OK;
}

