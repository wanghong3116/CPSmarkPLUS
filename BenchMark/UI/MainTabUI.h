// MFCDemoDlg.h : Í·ÎÄ¼þ
//

#pragma once

class CWorkMark;

class CMainTabUI:public DuiLib::CWindowWnd, DuiLib::INotifyUI
{

public:
	CMainTabUI();;
	~CMainTabUI();;
	LPCTSTR GetWindowClassName() const { return _T("UIDialog"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void LoadSkin();
	

	void	Notify(DuiLib::TNotifyUI& msg);

	void	OnNotifyWindowInit();

	void	OnNotifyClick(DuiLib::TNotifyUI& msg);
	void	OnClickButtonItemWork();
	void	OnClickOptionMain(const TCHAR* szOptionName);
	void	OnClickStartPageStart();
	void	OnClickResult();
	void	OnClickAbout();
	void	OnClickHelp();
	void	OnClickStopRunMark();
	//void	OnClickWorkMode(bool bBasicMode);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(UINT nID, BOOL& bHandled);
	LRESULT OnChangeRunningInfor(const TCHAR* szRunningInfor, BOOL& bHandled);
	LRESULT OnMarkWorkOver(BOOL& bHandled);
	LRESULT OnMarkWorkProgress(int nCode, int nValue, BOOL& bHandled);
	LRESULT OnMessageWorkMarkTypeName(const TCHAR* szMarkTypeName, BOOL& bHandled);
private:
	 
	int					StartWork();
	int					VerifyIsRunComprehensiveCalculation();
	void				SetStopRunButtonStatus(ERunMarkStatus eRMStatus);
	void				ResetCachedResultFromPrevRun();
	bool				SetFileName();
	void				SetLoopRunNumber();
	void				GetLoopRunNumber();
	void				SetMonitorPos();
 public:
	DuiLib::CPaintManagerUI										m_pm;
	DuiLib::CVerticalLayoutUI*									m_pverticalStaticDisplay = NULL;
	DuiLib::CVerticalLayoutUI*									m_pverticalRunningMonitor = NULL;
	DuiLib::CTextUI*											m_ptextCPUUtilization = NULL;
	DuiLib::CTextUI*											m_ptextCurrentWrokName = NULL;
	DuiLib::CButtonUI*											m_pbuttonMarkTypeName = NULL;
	DuiLib::CProgressUI *										m_pprogressMarkWork = NULL;
	DuiLib::CTextUI*											m_pbuttonRunItemIndex = NULL;
	DuiLib::CButtonUI*											m_pbuttonRunItemCount = NULL;
	DuiLib::CButtonUI*											m_pbuttonStopRunMark = NULL;
	DuiLib::CEditUI*											m_peditNumberOfRun;
	DuiLib::CEditUI*											m_peditCSVFileName;
	 
	DuiLib::CTextUI*											m_ptextTotalNumberOfRuns = NULL;
	DuiLib::CTextUI*											m_ptextCurrentNumberOfRun = NULL;
	DuiLib::CTextUI*											m_ptextVersion = NULL;
	int															m_nNumberOfRun = 1;
	DuiLib::CDuiString											m_strCSVFileName;
	 
	//DuiLib::CEditUI*											m_peditProjectName = NULL;
	//DuiLib::CEditUI*											m_peditThreadCount = NULL;
	DuiLib::CButtonUI*											m_pbuttonExit = NULL;
 	Mark::AuCpuUsage											m_cpuUsage;
	CWorkMark*													m_pWorkMark;
	CRect														m_rectWindowNormal;
	bool														m_bRunComprehensiveApplication = true;
	bool														m_bRunComprehensiveCalculation = true;
	bool														m_bCurrentRunIsComprehensiveApplication = true;
	ERunMarkStatus												m_eRunMarkStatus;
	int															m_nStopCountDownTime = 30;

	std::map<DuiLib::COptionUI*, DuiLib::CVerticalLayoutUI*>	m_mapOptionToVerticalPage;
	int m_currentRunNumber;
	int m_nLoopRunTime;
	static bool m_StartupRunFromUI;
};
