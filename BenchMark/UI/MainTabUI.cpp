// MFCDemoDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "MainTabUI.h"
#include "WorkMark.h"
#include "ResultDetailUI.h"
#include "InformationTrace.h"
#include <locale>

const TCHAR* g_pszMarkClassifyOptionName[E_MARK_CLASSIFY_END] = 
{
	_T("optionMedia"),		//	E_MARK_CLASSIFY_Comprehensive_Calculation = E_MARK_CLASSIFY_BEG,
	_T("optionOffice"),		//	E_MARK_CLASSIFY_Comprehensive_Application,
// 	_T("optionNet"),		//	E_MARK_CLASSIFY_NET,
// 	_T("optionStorage"),		//	E_MARK_CLASSIFY_STORAGE,
};

bool CMainTabUI::m_StartupRunFromUI = true;
CMainTabUI::CMainTabUI()
{
}

CMainTabUI::~CMainTabUI()
{
}

void CMainTabUI::LoadSkin()
{
	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;

	DuiLib::CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	{
		TCHAR szSkinDir[_MAX_PATH] = { 0 };

		MyPlatform::GetFullPathUnderExe(_T("skin"), szSkinDir);
		DuiLib::CPaintManagerUI::SetResourcePath(szSkinDir);
		//DuiLib::CPaintManagerUI::SetDefaultFont()
	}
	
	DuiLib::CControlUI* pRoot = builder.Create(_T("Main.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
}

void CMainTabUI::Notify(DuiLib::TNotifyUI& msg)
{
	if (msg.sType == _T("click")) 
	{
		OnNotifyClick(msg);
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnNotifyWindowInit();
	}
}

LRESULT CMainTabUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMainTabUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

LRESULT CMainTabUI::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainTabUI::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainTabUI::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainTabUI::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( NULL != m_pverticalRunningMonitor && m_pverticalRunningMonitor->IsVisible())
	{
		return HTCLIENT;
	}

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	DuiLib::CDuiRect rectExit = m_pbuttonExit->GetPos();
	if (pt.y < 100 && pt.x < rectExit.left)
	{
		return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT CMainTabUI::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) 
	{
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

LRESULT CMainTabUI::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CMainTabUI::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		else 
		{
			DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<DuiLib::CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CMainTabUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) 
	{
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_TIMER:			lRes = OnTimer((UINT)wParam, bHandled); break;
		case WS_CHANGE_RUNNING_INFOR: lRes = OnChangeRunningInfor((const TCHAR*)lParam, bHandled); break;
		case WS_MARK_WORK_OVER: lRes = OnMarkWorkOver(bHandled); break;
		case WS_MARK_WORK_PROGRESS: lRes = OnMarkWorkProgress(wParam, lParam, bHandled); break;
		case WS_MARK_TYPE_NAME: OnMessageWorkMarkTypeName((const TCHAR*)lParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainTabUI::OnNotifyClick(DuiLib::TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("buttonMainStartPage")
		|| msg.pSender->GetName() == _T("buttonMainItemPage")
		|| msg.pSender->GetName() == _T("buttonMainResultPage")
 		)
	{
		OnClickOptionMain(msg.pSender->GetName());

	}
	else if (msg.pSender->GetName() == _T("optionOffice"))
	{
		m_bRunComprehensiveApplication = !m_bRunComprehensiveApplication;
 	}
	else if (msg.pSender->GetName() == _T("optionMedia"))
	{
		m_bRunComprehensiveCalculation = !m_bRunComprehensiveCalculation;
 	}
	else if (msg.pSender->GetName() == _T("buttonMainExit"))
	{
		SendMessage(WM_CLOSE, 0, 0);
	}
	else if (msg.pSender->GetName() == _T("buttonItemWork"))
	{
 		if (!m_bRunComprehensiveApplication && !m_bRunComprehensiveCalculation)
			MessageBox(m_hWnd, _T("Please click on either Comprehensive Application or Calculation Button or Both."), _T("Warning"), MB_OK);
		else
		{
			ResetCachedResultFromPrevRun();
			if (SetFileName())
			{	TCHAR szLogDir[MAX_PATH] = {};
				MyPlatform::GetFullPathUnderExe(MyPlatform::ResultsFolder.c_str(), szLogDir, _MAX_PATH);
				TCHAR sysInfoLogFile[MAX_PATH] = {};
				_stprintf_s(sysInfoLogFile, _ARRAYSIZE(sysInfoLogFile), L"\"%s\"\\%s\\%s%s", szLogDir, (LPCWSTR)m_strCSVFileName,
					_T("SystemInfoLog"), _T(".txt"));
				 
				PROCESS_INFORMATION pi = { 0 };
				STARTUPINFO si = { sizeof(STARTUPINFO) };
				wchar_t szArgs[MAX_PATH] = _T("cmd.exe /c msinfo32.exe /report ");
				_tcscat_s(szArgs, sysInfoLogFile);
				if (FALSE == CreateProcess(NULL, szArgs, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
				{
					MyPlatform::MyTrace(L0, _T(" dump systeminfo CreateProcess failed, errno=[%d]"), GetLastError());
					return;
				}
				WaitForSingleObject(pi.hThread, INFINITE);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				 
				TCHAR powerLogFile[MAX_PATH] = {};
				_stprintf_s(powerLogFile, _ARRAYSIZE(powerLogFile), L"\"%s\"\\%s\\%s%s", szLogDir, (LPCWSTR)m_strCSVFileName,
					_T("PowerProfileLog"), _T(".txt"));
				PROCESS_INFORMATION pi1 = { 0 };
				STARTUPINFO si1 = { sizeof(STARTUPINFO) };
				wchar_t szArgs1[MAX_PATH] = _T("cmd.exe /c powercfg /qh >> ");
				_tcscat_s(szArgs1, powerLogFile);
				if (FALSE == CreateProcess(NULL, szArgs1, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si1, &pi1))
				{
					MyPlatform::MyTrace(L0, _T(" dump powercfg CreateProcess failed, errno=[%d]"), GetLastError());
					return;
				}
				WaitForSingleObject(pi1.hThread, INFINITE);
				CloseHandle(pi1.hThread);
				CloseHandle(pi1.hProcess);
				 
				GetLoopRunNumber();
				SetLoopRunNumber();
				OnClickButtonItemWork();
			}
			else
				MessageBox(m_hWnd, _T("Please set a file name."), _T("Warning"), MB_OK);
		}
	}
	else if (msg.pSender->GetName() == _T("buttonMainPageStartStart"))
	{
		OnClickStartPageStart();
	}
	else if (msg.pSender->GetName() == _T("buttonConfigure"))
	{
		ShellExecute(NULL, _T("open"), g_ini.GetFilePath(), NULL, NULL, SW_NORMAL);
	}
	else if (msg.pSender->GetName() == _T("buttonResult"))
	{
		OnClickResult();
		if (!m_StartupRunFromUI)
			exit(0);
	}
	else if (msg.pSender->GetName() == _T("buttonAbout"))
	{
		OnClickAbout();
	}
	else if (msg.pSender->GetName() == _T("buttonHelp"))
	{
		OnClickHelp();
	}
	else if (msg.pSender->GetName() == _T("buttonBasicMode"))
	{
		//OnClickWorkMode(true);
	}
	else if (msg.pSender->GetName() == _T("buttonProfessionalMode"))
	{
		//OnClickWorkMode(false);
	}
	else if (msg.pSender->GetName() == _T("buttonMonitorExit"))
	{
		exit(0);
	}
	else if (msg.pSender->GetName() == _T("buttonMonitorStop"))
	{
		OnClickStopRunMark();
	}
}

void CMainTabUI::ResetCachedResultFromPrevRun()
{
	m_nLoopRunTime = __max(1, std::stoi(std::wstring(m_peditNumberOfRun->GetText())));
	g_ini.SetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_NUMBER_OF_RUN, m_nLoopRunTime);
	g_ini.SetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_IS_APPLICATION, m_bRunComprehensiveApplication);
	g_ini.SetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_IS_CALCULATION, m_bRunComprehensiveCalculation);
	m_bCurrentRunIsComprehensiveApplication = m_bRunComprehensiveApplication ? 1 : 0;
	m_currentRunNumber = 1;
	g_ini.SetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_CURRENT_RUN_NUMBER, m_currentRunNumber);
	g_ini.SetString(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_CSV_FILE_NAME, _T(""));
	g_ini.SetString(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_PDF_FILE_NAME, _T(""));
	  
	for (size_t index = E_MARK_TYPE_BEG; index < E_MARK_TYPE_END; index++)
	{
		g_ini.SetIntW(CFG_APP_MARK_IS_RUN, std::to_wstring(index).c_str(), FALSE);
		g_ini.SetStringW(CFG_APP_MARK_RUN_TIME, std::to_wstring(index).c_str(), _T(""));
	}
}

void CMainTabUI::OnNotifyWindowInit()
{ 
	if (NULL == (m_pWorkMark = new CWorkMark()))
	{
		MessageBox(GetHWND(), _T("内存溢出"), _T("提示"), MB_OK);
		exit(1);
		return;
	}
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_AWAYMODE_REQUIRED);
	m_pWorkMark->SetHWnd(GetHWND());

	m_pverticalStaticDisplay	= (DuiLib::CVerticalLayoutUI*)m_pm.FindControl(_T("StaticDisplay"));
	m_pverticalRunningMonitor	= (DuiLib::CVerticalLayoutUI*)m_pm.FindControl(_T("RunningMonitor"));
	m_ptextCPUUtilization = (DuiLib::CTextUI*)m_pm.FindControl(_T("textCPUUtilization"));
	m_ptextCurrentWrokName = (DuiLib::CTextUI*)m_pm.FindControl(_T("textCurrentWrokName"));
	m_ptextTotalNumberOfRuns = (DuiLib::CTextUI*)m_pm.FindControl(_T("textTotalNumberOfRuns"));
	m_ptextCurrentNumberOfRun = (DuiLib::CTextUI*)m_pm.FindControl(_T("textCurrentNumberOfRun"));
	m_pprogressMarkWork = (DuiLib::CProgressUI *)m_pm.FindControl(_T("progressMarkWork"));
	m_ptextVersion = (DuiLib::CTextUI*)m_pm.FindControl(_T("textVersion"));
	if (NULL == m_pverticalStaticDisplay 
		|| NULL == m_pverticalRunningMonitor
		|| NULL == m_ptextCPUUtilization
		|| NULL == m_ptextCurrentWrokName
		|| NULL == m_pprogressMarkWork
		|| NULL == m_ptextVersion
		|| NULL == (m_pbuttonExit = (DuiLib::CButtonUI*)m_pm.FindControl(_T("buttonMainExit")))
		|| NULL == (m_pbuttonRunItemIndex = (DuiLib::CTextUI*)m_pm.FindControl(_T("buttonRunItemIndex")))
		|| NULL == (m_pbuttonMarkTypeName = (DuiLib::CButtonUI*)m_pm.FindControl(_T("buttonMarkTypeName")))
		|| NULL == (m_pbuttonStopRunMark = (DuiLib::CButtonUI*)m_pm.FindControl(_T("buttonMonitorStop")))
		|| NULL == (m_peditNumberOfRun = (DuiLib::CEditUI*)m_pm.FindControl(_T("editNumberOfRun")))
		|| NULL == (m_peditCSVFileName = (DuiLib::CEditUI*)m_pm.FindControl(_T("editCSVFileName")))
	 	)
	{
		MessageBox(GetHWND(), _T("未找到窗口节点"), _T("提示"), MB_OK);
		exit(1);
		return;
	}
	TCHAR version[_MAX_PATH] = { 0 };
	g_ini.GetString(CFG_SETTING, CFG_SETTING_PROJECT_VERSION, version, _MAX_PATH, _T("1.0"));
	GetWindowRect(GetHWND(), m_rectWindowNormal);
	m_ptextVersion->SetText(std::tstring(_T("ver: ")).append(version).c_str());
	VerifyIsRunComprehensiveCalculation();
	if (!m_StartupRunFromUI)
	{
		SetFileName();
		GetLoopRunNumber();
		SetLoopRunNumber();
		OnClickButtonItemWork();
	}
	else
	{
		CString strDefaultFileName = m_pWorkMark->DefaultFileNameByProjectId();
		m_peditCSVFileName->SetText(strDefaultFileName);
 	}
	 
}
void CMainTabUI::GetLoopRunNumber()
{
	m_currentRunNumber = g_ini.GetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_CURRENT_RUN_NUMBER, 1);
	m_nLoopRunTime = g_ini.GetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_NUMBER_OF_RUN);
}

void CMainTabUI::SetLoopRunNumber()
{
	g_ini.SetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_CURRENT_RUN_NUMBER, m_currentRunNumber);
	g_ini.SetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_NUMBER_OF_RUN, m_nLoopRunTime);
	m_pWorkMark->SetRunNumber(m_nLoopRunTime, m_currentRunNumber);
	if (m_ptextTotalNumberOfRuns != nullptr)
		m_ptextTotalNumberOfRuns->SetText(MyPlatform::Int64ToStringW(m_nLoopRunTime).c_str());
	if (m_ptextCurrentNumberOfRun != nullptr)
		m_ptextCurrentNumberOfRun->SetText(MyPlatform::Int64ToStringW(m_currentRunNumber).c_str());
 }

bool CMainTabUI::SetFileName()
{
	TCHAR SavedFileName[_MAX_PATH] = { 0 };
	g_ini.GetString(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_CSV_FILE_NAME, SavedFileName, _MAX_PATH);
	if (_tcslen(SavedFileName) > 0)
	{
		m_peditCSVFileName->SetText(SavedFileName);
		m_strCSVFileName = SavedFileName;
		if (m_strCSVFileName == _T("")) return false;
		m_pWorkMark->SetCSVFileName(m_strCSVFileName);
		m_pWorkMark->LogInfo({ _T("Inside SetFileName Use Saved File Name from init = "), tstring(m_strCSVFileName).c_str() });
	}
	else
	{
		tstring CurName = m_peditCSVFileName->GetText();
		if (CurName.length() == 0)
		{
			CurName = m_pWorkMark->DefaultFileNameByProjectId();
			m_strCSVFileName = CurName.c_str();
			if (m_strCSVFileName == _T("")) return false;
			m_pWorkMark->SetCSVFileName(m_strCSVFileName);
			m_pWorkMark->LogInfo({ _T("Inside SetFileName  use default file name with date info "), tstring(m_strCSVFileName).c_str() });
		}
		else
		{
			m_strCSVFileName = CurName.c_str();
			if (m_strCSVFileName == _T("")) return false;
			m_pWorkMark->SetCSVFileName(m_strCSVFileName);
			m_pWorkMark->LogInfo({ _T("Inside SetFileName  use saved file name from last run = "), tstring(m_strCSVFileName).c_str() });
		}
	}	
	g_ini.SetString(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_CSV_FILE_NAME, m_strCSVFileName);
	return true;
}
void CMainTabUI::OnClickButtonItemWork()
{
	StartWork();	 
	return;
}

LRESULT CMainTabUI::OnTimer(UINT nID, BOOL& bHandled)
{
	bHandled = TRUE;
	switch (nID)
	{
		case IDT_TIME_CPU_UTILIZATION:
			if (NULL == m_ptextCPUUtilization)
			{
				bHandled = FALSE;
				return FALSE;
			}
			else
			{
				// 	TCHAR szText[_MAX_PATH] = { 0 };
				// 
				// 	_stprintf_s(szText, _ARRAYSIZE(szText), _T("CPU使用率%d%%"), m_cpuUsage.Get());
				// 	m_ptextCPUUtilization->SetText(szText);
				SetMonitorPos();
				SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				break;
			}
		case IDT_STOP_COUNT_DOWN:
			if (E_RMS_COUNT_DOWN == m_eRunMarkStatus)
			{
				m_nStopCountDownTime--;
				SetStopRunButtonStatus(E_RMS_COUNT_DOWN);
				if (m_nStopCountDownTime <= 0)
				{
					KillTimer(m_hWnd, IDT_STOP_COUNT_DOWN);
					SetStopRunButtonStatus(E_RMS_RUN);
				}
			}		
		default:
			break;
	}
	return TRUE;
}

LRESULT CMainTabUI::OnChangeRunningInfor(const TCHAR* szRunningInfor, BOOL& bHandled)
{
	bHandled = TRUE;
	m_ptextCurrentWrokName->SetText(szRunningInfor);
	return TRUE;
}

LRESULT CMainTabUI::OnMarkWorkOver(BOOL& bHandled)
{
	bHandled = true;
	m_pWorkMark->WaitForWorkThread();
	KillTimer(this->GetHWND(), IDT_TIME_CPU_UTILIZATION);
	m_bRunComprehensiveApplication = g_ini.GetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_IS_APPLICATION) == 1 ? true : false;
	m_bRunComprehensiveCalculation =  g_ini.GetInt(CFG_LOOPRUNINFO, CFG_LOOPRUNINFO_IS_CALCULATION) == 1 ? true : false;
	if (m_bRunComprehensiveApplication && m_bRunComprehensiveCalculation)
	{
		if (m_bCurrentRunIsComprehensiveApplication == 1)
		{
			m_pWorkMark->RecordAllMarkResult();
			m_pWorkMark->Reboot();
		}
		else if (m_currentRunNumber < m_nLoopRunTime)
		{
			m_pWorkMark->RecordAllMarkResult();
	 		m_bCurrentRunIsComprehensiveApplication = 1;
 		}
		else
		{
			m_pWorkMark->ReadAppMarkResultFromDisk(); 
			m_pWorkMark->RecordAllMarkResult();
		}
	}
	else if (m_bRunComprehensiveCalculation && (m_currentRunNumber < m_nLoopRunTime))
	{
		m_pWorkMark->ReadAppMarkResultFromDisk();
		m_pWorkMark->RecordAllMarkResult();
		m_currentRunNumber += 1;
		SetLoopRunNumber();
		m_pWorkMark->Reboot();
	}
	else if (m_bRunComprehensiveApplication)
	{
		m_bCurrentRunIsComprehensiveApplication = 1;
		m_pWorkMark->RecordAllMarkResult();
	}

	if (m_nLoopRunTime == m_currentRunNumber)
	{
		m_pWorkMark->LogInfo({ _T("所有测试项目已经全部执行完毕!!!!") });
		m_pverticalRunningMonitor->SetVisible(false);
		m_pverticalStaticDisplay->SetVisible(true);
		SetWindowPos(GetHWND(), HWND_BOTTOM, 0, 0, m_rectWindowNormal.Width(), m_rectWindowNormal.Height(), SWP_NOMOVE);
		CenterWindow();
		SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		OnClickResult();
	}
	else
	{
		m_currentRunNumber += 1;
		SetLoopRunNumber();
		if (0 == StartWork())
		{
			return TRUE;
		}
	}
	return TRUE;
}

LRESULT CMainTabUI::OnMarkWorkProgress(int nCode, int nValue, BOOL& bHandled)
{
	bHandled = TRUE;
	m_pbuttonRunItemIndex->SetText(m_bCurrentRunIsComprehensiveApplication ? _T("Application: ") : _T("Calculation: "));
	switch (nCode)
	{
		case 0:
		{
			m_pprogressMarkWork->SetMaxValue(nValue);
			m_pprogressMarkWork->SetMinValue(0);
			m_pprogressMarkWork->SetValue(nValue / 2);
		}
		break;

		case 1:
		{
	 		m_pprogressMarkWork->SetValue(nValue);
			m_nStopCountDownTime = 5;
			SetStopRunButtonStatus(E_RMS_COUNT_DOWN);
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
	m_pprogressMarkWork->SetVisible();
	return BENCH_MARK_OK;
}

void CMainTabUI::OnClickOptionMain(const TCHAR* szOptionName)
{
	DuiLib::COptionUI* pOption = (DuiLib::COptionUI*)m_pm.FindControl(szOptionName);

	std::map<DuiLib::COptionUI*, DuiLib::CVerticalLayoutUI*>::const_iterator it = m_mapOptionToVerticalPage.begin();
	for (; it != m_mapOptionToVerticalPage.end(); it++)
	{
		it->first->Selected(false);
		it->second->SetVisible(false);
	}
	m_mapOptionToVerticalPage[pOption]->SetVisible(true);
}

void CMainTabUI::OnClickStartPageStart()
{
	DuiLib::COptionUI *poptionResult = (DuiLib::COptionUI*)m_pm.FindControl(_T("buttonMainItemPage"));
	poptionResult->SetVisible(true);
	OnClickOptionMain(_T("buttonMainItemPage"));
	poptionResult->Selected(true);
}
 
void CMainTabUI::OnClickResult()
{
	const std::vector<MarkWorkRunningInfor>* pvMWRInfor = NULL;
	CString strDefaultFileName;
	CString szPDFFileName = m_strCSVFileName ;

	if (NULL == m_pWorkMark
		|| NULL == (pvMWRInfor = m_pWorkMark->GetMarkRunInfor()))
	{
		tstring errorMsg;
		if (NULL == m_pWorkMark)
			errorMsg = _T("WorkMark is null.");
		else
			errorMsg = _T("WorkMark has no running info.");
		m_pWorkMark->LogInfo({ errorMsg });
		return;
	}

	ShowWindow(false);	 
	CResultDetailUI resultDetailUI(GetHWND(), pvMWRInfor, false, szPDFFileName, m_pWorkMark);
	 
	resultDetailUI.DoModal();
	ShowWindow(true);
 	return;
}

void CMainTabUI::OnClickAbout()
{
	ShellExecute(NULL, _T("open"), _T("http://www.nim.ac.cn/"), NULL, NULL, SW_SHOW);
	return;
}

void CMainTabUI::OnClickHelp()
{
	TCHAR szHelpFile[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("help.txt"), szHelpFile, _MAX_PATH);
	ShellExecute(NULL, _T("open"), szHelpFile, NULL, NULL, SW_NORMAL);
	return;
}
 
int CMainTabUI::StartWork()
{
	if (m_bCurrentRunIsComprehensiveApplication)
		m_pbuttonMarkTypeName->SetText(_T("办公测试"));
	else
		m_pbuttonMarkTypeName->SetText(_T("计算测试"));
	int nClassifyIdx = (m_bCurrentRunIsComprehensiveApplication) ? E_MARK_CLASSIFY_Comprehensive_Application : E_MARK_CLASSIFY_Comprehensive_Calculation;
	m_pWorkMark->SetMarkClassifyItemSelect((EMarkClassify)nClassifyIdx, true);

	m_pWorkMark->WaitForWorkThread();
	// 	for (int nClassifyIdx = E_MARK_CLASSIFY_BEG; nClassifyIdx < E_MARK_CLASSIFY_END; nClassifyIdx++)
	// 	{
	// 		DuiLib::COptionUI* pOption = NULL;
	// 
	// 		if (NULL == (pOption = (DuiLib::COptionUI*)m_pm.FindControl(g_pszMarkClassifyOptionName[nClassifyIdx])))
	// 		{
	// 			return -1;
	// 		}
	// 		m_pWorkMark->SetMarkClassifyItemSelect((EMarkClassify)nClassifyIdx, true);
	// 	}
	//m_pWorkMark->SetMarkModel(m_bBasicModel);
	//g_ini.SetInt(CFG_CPU, CFG_CPU_THREAD_COUNT, _tstoi(m_peditThreadCount->GetText()));
	//g_ini.SetString(CFG_SETTING, CFG_SETTING_PROJECT_NAME, m_peditProjectName->GetText());
	SetStopRunButtonStatus(E_RMS_RUN);
	m_pWorkMark->SetProjectName(_T(""));
	if (BENCH_MARK_OK != m_pWorkMark->StartWork(m_bCurrentRunIsComprehensiveApplication))
	{
		return -2;
	}
	SetMonitorPos();
	m_pverticalStaticDisplay->SetVisible(false);
	m_pverticalRunningMonitor->SetVisible(true);
	SetTimer(this->GetHWND(), IDT_TIME_CPU_UTILIZATION, 500, NULL);
	return 0;
}

void CMainTabUI::SetMonitorPos()
{
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int nMonitorW = 0, nMonitorH = 0;

	nMonitorW = m_pverticalRunningMonitor->GetFixedWidth();
	nMonitorH = m_pverticalRunningMonitor->GetFixedHeight();
	::SetWindowPos(m_hWnd, NULL, nScreenWidth - nMonitorW, 0, nMonitorW, nMonitorH, SWP_NOZORDER | SWP_NOACTIVATE);
}

int CMainTabUI::VerifyIsRunComprehensiveCalculation()
{
	LPWSTR *szArglist = NULL;
	int nArgs = 0;
	int nOffset = 0;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	for (nOffset = 1; nOffset < nArgs; nOffset++)
	{
		USES_CONVERSION;
		if (0 == _tcsicmp(REBOOT_RUN_COMPREHENSIVE_CALC_ARGUEMENT, W2T(szArglist[nOffset])))
		{
			m_bCurrentRunIsComprehensiveApplication = false;
			m_StartupRunFromUI = false;
			break;
		}
	}
	return 0;
}

void CMainTabUI::SetStopRunButtonStatus(ERunMarkStatus eRMStatus)
{
	TCHAR szText[_MAX_PATH] = { 0 };

	m_eRunMarkStatus = eRMStatus;
	m_pWorkMark->SetRunMarkStatus(eRMStatus);
	switch (eRMStatus)
	{
		case E_RMS_RUN:
			m_pbuttonStopRunMark->SetVisible(false);
			break;
		case E_RMS_STOP:
			m_pbuttonStopRunMark->SetVisible(true);
			_stprintf_s(szText, _T("继续"));
			break;
		case E_RMS_COUNT_DOWN:
			_stprintf_s(szText, _T("暂停(%d)"), m_nStopCountDownTime);
			m_pbuttonStopRunMark->SetVisible(true);
			SetTimer(m_hWnd, IDT_STOP_COUNT_DOWN, 1000, NULL);
			break;
		case E_RUNMARKSTATUS_END:
			break;
		default:
			break;
	}
	m_pbuttonStopRunMark->SetText(szText);
}

void CMainTabUI::OnClickStopRunMark()
{
	switch (m_eRunMarkStatus)
	{
		case E_RMS_STOP:
			KillTimer(m_hWnd, IDT_STOP_COUNT_DOWN);
			SetStopRunButtonStatus(E_RMS_COUNT_DOWN);
			break;
		case E_RMS_COUNT_DOWN:
			SetStopRunButtonStatus(E_RMS_STOP);
			break;
		default:
			SetStopRunButtonStatus(E_RMS_RUN);
			break;
	}
}

LRESULT CMainTabUI::OnMessageWorkMarkTypeName(const TCHAR* szMarkTypeName, BOOL& bHandled)
{
	bHandled = TRUE;
	m_pbuttonMarkTypeName->SetText(szMarkTypeName);
	return 0;
}