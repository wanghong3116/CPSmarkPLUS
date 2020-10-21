// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ResultDetailUI.h"
#include "MarkObject.h"
#include "PremiereMark.h"
#include "Photoshop.h"
#include "ChromeMark.h"
#include "OfficeWordMark.h"
#include "OfficeExcelMark.h"
#include "OfficePowerPointMark.h"
//#include "ZipMark.h"
#include "VideoPlayer.h"
#include "AEMark.h"
#include "HandBrake.h"
//#include "OpenCL.h"
#include "3DSMax.h"
#include "Resource.h"
#include "WinrarZipMark.h"
#include "PDFMark.h"
#include "3DSMaxWhale.h"
#include "ResultReport.h"
//#include "3DSMaxCar.h"
//#include "3DSMaxHome.h"
#include "InformationTrace.h"

#define IDT_TIME_CPU_UTILIZATION 1001
const DWORD TEXT_COLOR = 0xff646464;
const TCHAR* TEXT_NAME_TOTAL_MARK_TIME = _T("textTotalMarkTime");

const TCHAR* g_pszButtonNameMarkClassifyTime[E_MARK_CLASSIFY_END] = {
	_T("buttonMediaTime"),//E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	_T("buttonOfficeTime"),  //E_MARK_CLASSIFY_Comprehensive_Application,
// 	_T("buttonNetTime"),	//E_MARK_CLASSIFY_NET,
// 	_T("buttonStorageTime"), //E_MARK_CLASSIFY_STORAGE,
};
const double g_pdAchievementK[E_MARK_CLASSIFY_END] = {
#ifdef _DEBUG
	679599, // 681640, //3481.06, //2987.56,//E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	// 213685, // 791.56, //1673.809,  //E_MARK_CLASSIFY_Comprehensive_Application,
#else
	679599, //681640, //3481.06, //2987.56,//E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	214826// 213685, // 791.56, //1673.809,  //E_MARK_CLASSIFY_Comprehensive_Application,
#endif
};

const double g_pnClassifyMarkCount[E_MARK_CLASSIFY_END] = {
#ifdef _DEBUG
	1,//E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	1,  //E_MARK_CLASSIFY_Comprehensive_Application,
#else
	4,//E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	9,  //E_MARK_CLASSIFY_Comprehensive_Application,
#endif
};

TCHAR* g_pszAchievementTextFormat[E_MARK_CLASSIFY_END] = {
	_T("%d"),  //E_MARK_CLASSIFY_Comprehensive_Application,
	_T("%d"),//E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
};

const TCHAR* g_szButtonTotalTime = _T("buttonTotalTime");
const UINT_PTR REPORT_GENERATE_WAIT_TIMER_ID = 0xffffAA;
const TCHAR* BUTTON_NAME_MAIN_PAGE = _T("buttonReturnMainPage");
const TCHAR* BUTTON_NAME_EXIT = _T("buttonExit");

CResultDetailUI::CResultDetailUI(HWND hParent, const std::vector<MarkWorkRunningInfor>* pvMWRInfor, bool bBasicModel, const TCHAR* szPDFFileName, CWorkMark* pworkMark) :
	m_hParent(hParent),
	m_pvMarkWorkRunInfor(pvMWRInfor),
	m_bBasicModel(bBasicModel),
	m_pworkMark(pworkMark)
{
	_tcscpy_s(m_szPDFFileName, _ARRAYSIZE(m_szPDFFileName), szPDFFileName);
	if (MyPlatform::CInformationTrace::IsInTestMode == 1)
	{
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_NET, _T("CHROME")  });
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_HANDBRAKE, _T("HANDBRAKE") });
	}
	else
	{
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_NET, _T("CHROME") });
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_OUTLOOK, _T("OUTLOOK") });
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_OFFICE_EXCEL, _T("OFFICE_EXCEL") });
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_OFFICE_POWERPOINT, _T("OFFICE_POWERPOINT")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_OFFICE_WORD, _T("OFFICE_WORD")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_PDF, _T("PDF")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_PHOTOSHOP, _T("PHOTOSHOP")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_WINRAR, _T("WINRAR")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_CAD, _T("AUTOCAD")});

		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_3DSMAX_WHALE, _T("3DSMAX_WHALE")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_AE, _T("AE")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_PREMIERE, _T("PREMIERE")});
		m_mapItemToName.insert(pair<EMarkType, std::tstring>{E_MARK_HANDBRAKE, { _T("HANDBRAKE") }});
	}
}

CResultDetailUI::~CResultDetailUI()
{
}

void CResultDetailUI::LoadSkin()
{	
}

void CResultDetailUI::Notify(DuiLib::TNotifyUI & msg)
{
	DuiLib::CDuiString strType = msg.sType;

	if (strType == _T("click"))
	{
		DuiLib::CDuiString strName = msg.pSender->GetName();

		if (strName == BUTTON_NAME_MAIN_PAGE)
		{
			Close(0);
		}
		else if (strName == BUTTON_NAME_EXIT)
		{
			exit(0);
		}
		
	}
	else if (strType == _T("setfocus"))
	{
	}
	else if (strType == _T("windowinit"))
	{
		OnNotifyWindowInit();
	}
}

LRESULT CResultDetailUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CResultDetailUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

LRESULT CResultDetailUI::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CResultDetailUI::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CResultDetailUI::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CResultDetailUI::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (pt.y < 260 && pt.x < 1000)
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

LRESULT CResultDetailUI::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CResultDetailUI::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CResultDetailUI::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) 
	{
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

LRESULT CResultDetailUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
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
		case WM_TIMER:			lRes = OnTimer((UINT_PTR)wParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

int CResultDetailUI::DoModal()
{
	Create(m_hParent, NULL, UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE | WS_EX_APPWINDOW);
	ShowWindow(true);
	
	SetIcon(IDR_MAINFRAME);
	ShowModal();
	DuiLib::CPaintManagerUI::MessageLoop();
	return TRUE;
}

LRESULT CResultDetailUI::OnCreate(BOOL& bHandled)
{
	bHandled = FALSE;

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	m_pRoot = builder.Create(_T("ResultDetail.xml"), (UINT)0, NULL, &m_pm);
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

void CResultDetailUI::OnNotifyWindowInit()
{
	__int64 pnTime[E_MARK_CLASSIFY_END] = { 0 };
	double dAchievement = 0;
	TCHAR szText[_MAX_PATH] = { 0 };
 
	for (int nClassifyIndex = E_MARK_CLASSIFY_BEG; nClassifyIndex < E_MARK_CLASSIFY_END; nClassifyIndex++)
	{
		if (NULL == (m_ppbuttonMarkClassifyTime[nClassifyIndex] = (DuiLib::CButtonUI*)m_pm.FindControl(g_pszButtonNameMarkClassifyTime[nClassifyIndex])))
		{
			continue;
		}

		m_pnAchievement[nClassifyIndex] = (int)CalcAchievement((EMarkClassify)nClassifyIndex);
		if (0 == m_pnAchievement[nClassifyIndex])
		{
			_tcscpy_s(szText, _ARRAYSIZE(szText), _T("负载运行错误"));
		}
		else
		{
			_stprintf_s(szText, _ARRAYSIZE(szText), g_pszAchievementTextFormat[nClassifyIndex], m_pnAchievement[nClassifyIndex]);
		}
		
		m_ppbuttonMarkClassifyTime[nClassifyIndex]->SetText(szText);
	}
	
	//if(m_ppbuttonMarkClassifyTime[E_MARK_CLASSIFY_Comprehensive_Calculation] != nullptr)
		m_ppbuttonMarkClassifyTime[E_MARK_CLASSIFY_Comprehensive_Calculation]->SetVisible(false);

	//if (m_ppbuttonMarkClassifyTime[E_MARK_CLASSIFY_Comprehensive_Application] != nullptr)
	//{
		m_ppbuttonMarkClassifyTime[E_MARK_CLASSIFY_Comprehensive_Application]->SetVisible(true);
	//}
	

	int nFinalAchievement = 0;
	if((0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation])
		&& (0 == m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application]))
	{
		nFinalAchievement = m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation];
	}
	else if ((0 == m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation])
		&& (0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application]))
	{
		nFinalAchievement = m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application];
	}
	else if ((0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation])
		&& (0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application]))
	{
		nFinalAchievement = m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation] * 
			m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application];

		nFinalAchievement = ::sqrt(nFinalAchievement);
	}
	
	if (nFinalAchievement == 0)
	{
		_tcscpy_s(szText, _ARRAYSIZE(szText), _T("负载运行错误"));
	}
	else
	{
		_stprintf_s(szText, _ARRAYSIZE(szText), 
			g_pszAchievementTextFormat[E_MARK_CLASSIFY_Comprehensive_Calculation], 
			nFinalAchievement);
	}

	m_ppbuttonMarkClassifyTime[E_MARK_CLASSIFY_Comprehensive_Application]->SetText(szText);
	
	

	RECT rect;
	GetClientRect(GetHWND(), &rect);
	::SetWindowPos(GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	DisplayComputerInformation();

	if (NULL == (m_pButtonWait = (DuiLib::CButtonUI*)m_pm.FindControl(_T("buttonWaitReport")))
		|| NULL == (m_pButtonMainPage = (DuiLib::CButtonUI*)m_pm.FindControl(BUTTON_NAME_MAIN_PAGE))
		|| NULL == (m_pButtonExit = (DuiLib::CButtonUI*)m_pm.FindControl(BUTTON_NAME_EXIT))
		)
	{
		return;
	}
	m_bGenerageReportOver = false;
	m_pButtonWait->SetVisible();
	::SetTimer(GetHWND(), REPORT_GENERATE_WAIT_TIMER_ID, 1000, NULL);
	m_threadGenerateReport.swap(std::thread([](CResultDetailUI* pRDUI)->void {
		pRDUI->GenerateReportThread();
	}, this));
}

double CResultDetailUI::CalcAchievement(EMarkClassify eMarkClassify)
{
	//return 1350;
	int nCount = 0;
	//double dProduct = 1;
	double average = 0;

	for (size_t nMarkIndex = 0; nMarkIndex < m_pvMarkWorkRunInfor->size(); nMarkIndex++)
	{
		const MarkWorkRunningInfor& mwri = (*m_pvMarkWorkRunInfor)[nMarkIndex];

		if (eMarkClassify == mwri.eMarkClassify)
		{
			double sum = 0;
			for each (auto& s in mwri.nRunningTime)
				sum += s;
			average += (sum / mwri.nRunningTime.size());
			//dProduct = dProduct * ((double)average / (double)10000);
			nCount++;
			
			//dProduct = dProduct * ((double)mwri.nRunningTime / (double)10000);
			//nCount++;
		}
	}
	if (g_pnClassifyMarkCount[eMarkClassify] != nCount)
	{
		return 0;
	}

	return (g_pdAchievementK[eMarkClassify] * 1000) / average;
}

extern std::string GetComputerCPUName();
extern std::vector<DWORD>  GetComputerCPUDominantFrequency(DWORD dwPhysicalProcessorNumber);
extern int GetComputerCPUNumber(DWORD& processorPackageCount, DWORD& processorCoreCount, DWORD& logicalProcessorCount);
extern std::string GetComputerCPUIdentifier();
extern double GetComputerMemorySize();
extern DWORD GetComputerAdapterCount();
extern int GetComputerAdapterDescription(std::vector<std::string>& rvstrAdapterDescription);
extern std::tstring GetComputerOSNameAndVersion();

int CResultDetailUI::SetTextUIText(const TCHAR* szUIName, const TCHAR* szText)
{
	DuiLib::CTextUI* pTextUI = NULL;

	if (NULL == (pTextUI = (DuiLib::CTextUI*)m_pm.FindControl(szUIName)))
	{
		return -1;
	}
	pTextUI->SetText(szText);
	return BENCH_MARK_OK;
}

int CResultDetailUI::DisplayComputerInformation()
{
	DWORD dwPhysicalProcessorNumber = 0;
	DWORD dwProcessorCoreNUmber = 0;
	DWORD dwProcessorLogicalNumber = 0;
	std::string strCpuName;
 
	strCpuName = GetComputerCPUName();
	GetComputerCPUNumber(dwPhysicalProcessorNumber, dwProcessorCoreNUmber, dwProcessorLogicalNumber);
	std::vector<DWORD> vec_dwDominatFrequency = {};
	vec_dwDominatFrequency = GetComputerCPUDominantFrequency(dwPhysicalProcessorNumber);
	for each (auto dw in vec_dwDominatFrequency)
		_tcscat_s(m_szCPUHz, _ARRAYSIZE(m_szCPUHz), ((MyPlatform::DoubleToStringW((dw / 10 * 10) / (double)1000, 2))  + _T("/")).c_str());
	 
	_tcscat_s(m_szCPUHz, _ARRAYSIZE(m_szCPUHz), _T(" GHz"));
	SetTextUIText(_T("textCPUHz"), m_szCPUHz);
	_tcscpy_s(m_szCPUName, _ARRAYSIZE(m_szCPUName), MyPlatform::CharToTchar(strCpuName.c_str()).c_str());
	SetTextUIText(_T("textCPUName"), m_szCPUName);
	_tcscpy_s(m_szCPUNumber, _ARRAYSIZE(m_szCPUNumber), (MyPlatform::Int64ToString(dwPhysicalProcessorNumber) + _T(" 物理核心/ ") + MyPlatform::Int64ToString(dwProcessorCoreNUmber) + _T(" 核心/ ") + MyPlatform::Int64ToString(dwProcessorLogicalNumber) + _T(" 线程")).c_str());
	SetTextUIText(_T("textCPUThreadNum"), m_szCPUNumber);
	_tcscpy_s(m_szMemory, _ARRAYSIZE(m_szMemory), (MyPlatform::Int64ToString((int)(GetComputerMemorySize() + 0.5)) + _T(" GB")).c_str());
	SetTextUIText(_T("textMemory"), m_szMemory);
	
  //AddInformationLayout(pRightReust, _T("CPU 命令集"), MyPlatform::CharToTchar(GetComputerCPUIdentifier().c_str()).c_str());
 
	int nAdapterCount = 0;
	std::vector<std::string> vstrAdapterDesc;
	nAdapterCount = GetComputerAdapterCount();
	//AddInformationLayout(pRightReust, _T("显卡数目"), MyPlatform::Int64ToString(nAdapterCount).c_str());
	GetComputerAdapterDescription(vstrAdapterDesc);
	for (int nAdapterIdx = 0; nAdapterIdx < nAdapterCount; nAdapterIdx++)
	{
		//TCHAR szLeft[_MAX_PATH] = { 0 };

		//_stprintf_s(szLeft, _ARRAYSIZE(szLeft), _T("显卡%d"), nAdapterIdx + 1);
		_tcscpy_s(m_szGPU, _ARRAYSIZE(m_szGPU), MyPlatform::CharToTchar(vstrAdapterDesc[nAdapterIdx].c_str()).c_str());
		SetTextUIText(_T("textGPU"), m_szGPU);
		//AddInformationLayout(pRightReust, szLeft, MyPlatform::CharToTchar(vstrAdapterDesc[nAdapterIdx].c_str()).c_str());
	}
	
	TCHAR temp[_MAX_PATH] = { 0 };
	g_ini.GetString(CFG_SETTING, CFG_SETTING_PROJECT_NAME, temp, _MAX_PATH );
	_tcscpy_s(m_szProjectNameAndVersion, _ARRAYSIZE(m_szProjectNameAndVersion), temp);
	g_ini.GetString(CFG_SETTING, CFG_SETTING_PROJECT_VERSION, temp, _MAX_PATH);
	_tcscat_s(m_szProjectNameAndVersion, _ARRAYSIZE(m_szProjectNameAndVersion), temp);
	_tcscpy_s(m_szOSNameAndVersion, _ARRAYSIZE(m_szOSNameAndVersion), GetComputerOSNameAndVersion().c_str());
	return BENCH_MARK_OK;
}

int CResultDetailUI::GenerateReportThread()
{
	ResultReport reportGenerate( m_pvMarkWorkRunInfor, m_pnAchievement, m_szCPUName, m_szCPUHz, m_szCPUNumber, m_szMemory, m_szGPU, 
		m_szOSNameAndVersion, m_szPDFFileName, m_pworkMark, &m_mapItemToName);

	if (BENCH_MARK_OK == reportGenerate.init())
	{
		reportGenerate.Generate();
	}
	reportGenerate.destroy();
	
	m_bGenerageReportOver = true;
	return BENCH_MARK_OK;
}

LRESULT CResultDetailUI::OnTimer(UINT_PTR nTimerID, BOOL& bHandled)
{
	bHandled = FALSE;
	switch (nTimerID)
	{
		case REPORT_GENERATE_WAIT_TIMER_ID:
			bHandled = TRUE;
			OnTimerGenerateReport();
			break;
		default:
			break;
	}
	return TRUE;
}

int CResultDetailUI::OnTimerGenerateReport()
{
	static int s_nPointNumber = 1;	
	TCHAR szText[_MAX_PATH] = { 0 };

	if (m_bGenerageReportOver)
	{
		m_threadGenerateReport.join();
		KillTimer(GetHWND(), REPORT_GENERATE_WAIT_TIMER_ID);
		m_pButtonWait->SetVisible(false);
		m_pButtonMainPage->SetEnabled(true);
		CenterWindow();
	}
	else
	{
		m_pButtonWait->SetVisible(true);
		m_pButtonMainPage->SetEnabled(false);

		_stprintf_s(szText, _T("%s"), _T("报告正在生成中"));
		for (int nIndex = 0; nIndex < s_nPointNumber; nIndex++)
		{
			_tcscat_s(szText, _T("."));
		}
		m_pButtonWait->SetText(szText);

		s_nPointNumber++;
		if (s_nPointNumber > 10)
		{
			s_nPointNumber = 1;
		}
	}
	return 0;
}