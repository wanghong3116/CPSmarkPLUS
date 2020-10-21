// MFCDemoDlg.h : Í·ÎÄ¼þ
//

#pragma once

class CMonitorUI : public DuiLib::CWindowWnd, DuiLib::INotifyUI
{


	friend int __stdcall MarkStepInformationCallback(void* pParam, const TCHAR* szFormat, ...);

	template<class type1, class type2>
	int NewMarkObjectAndAdd(std::vector<type1*>& rvMark, const TCHAR* szKey, type2* newType);
public:
	CMonitorUI(HWND hParent);;
	~CMonitorUI();;

	int DoModal();

	LPCTSTR GetWindowClassName() const { return _T("UIDialog"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void LoadSkin();
	

	void Notify(DuiLib::TNotifyUI& msg);
	void OnNotifyBnClick(DuiLib::TNotifyUI & msg);
	void OnNotifyWindowInit();

	void OnBnClickTotalLevel();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT nID, BOOL& bHandled);
	LRESULT OnChangeRunningInfor(const TCHAR* szRunningInfor, BOOL& bHandled);
	LRESULT OnMarkWorkOver(BOOL& bHandled);
	LRESULT OnMarkWorkProgress(int nCode, int nValue, BOOL& bHandled);
private:
	static DWORD WINAPI ThreadProcMarkWork(_In_ LPVOID lpParameter);
	DWORD MarkWork();
	int AddRunInformation(TCHAR* szFormat, ...);
	
	int SetLabelTextByName(const TCHAR* szLabelName, const TCHAR* szText);
private:
	
	DuiLib::CControlUI*				m_pRoot = NULL;
	DuiLib::CVerticalLayoutUI*		m_pverticalMonitor = NULL;
	DuiLib::CHorizontalLayoutUI*	m_pverticalResult = NULL;
	DuiLib::CTextUI*				m_ptextCPUUtilization = NULL;
	DuiLib::CTextUI*				m_ptextCurrentWrokName = NULL;
	DuiLib::CProgressUI *			m_pprogressMarkWork = NULL;

	FILE *							m_pfileRunningInfor = NULL;
	std::vector<MarkWorkRunningInfor> m_vMarkWorkRunInfor;

	DuiLib::CPaintManagerUI			m_pm;
	HWND							m_hParent;
	Mark::AuCpuUsage				m_cpuUsage;

	BOOL							m_bExitThreadMark = TRUE;
	HANDLE							m_hThreadMark = NULL;
};

template<class type1, class type2>
int CMonitorUI::NewMarkObjectAndAdd(std::vector<type1*>& rvMark, const TCHAR* szKey, type2* newType)
{
	if (!g_ini.GetInt(CFG_MARK, szKey))
	{
		return -1;
	}

	type2* pNew = new type2();
	if (NULL == pNew)
	{
		return -2;
	}

	rvMark.push_back(pNew);
	return BENCH_MARK_OK;
}
