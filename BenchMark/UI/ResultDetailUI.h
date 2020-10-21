// MFCDemoDlg.h : Í·ÎÄ¼þ
//

#pragma once
#include "WorkMark.h"
class CResultDetailUI : public DuiLib::CWindowWnd, DuiLib::INotifyUI
{
public:
	CResultDetailUI(HWND hParent, const std::vector<MarkWorkRunningInfor>* pvMWRInfor, bool bBasicModel, const TCHAR* szPDFFileName,  CWorkMark* pworkMark);
	~CResultDetailUI();;

	int DoModal();

	LPCTSTR GetWindowClassName() const { return _T("UIDialog"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void LoadSkin();
	

	void Notify(DuiLib::TNotifyUI& msg);
	void OnNotifyWindowInit();

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
	LRESULT OnTimer(UINT_PTR nTimerID, BOOL& bHandled);
private:

	int SetTextUIText(const TCHAR* szUIName, const TCHAR* szText);
	int DisplayComputerInformation();

	double CalcAchievement(EMarkClassify eMarkClassify);

	int GenerateReportThread();

	int OnTimerGenerateReport();
  
private:
	
	DuiLib::CButtonUI*				m_ppbuttonMarkClassifyTime[E_MARK_CLASSIFY_END] = { NULL };
	DuiLib::CButtonUI*				m_pbuttonTotalTime = NULL;
	DuiLib::CButtonUI*				m_pButtonWait = NULL;
	DuiLib::CButtonUI*				m_pButtonMainPage = NULL;
	DuiLib::CButtonUI*				m_pButtonExit = NULL;
	DuiLib::CControlUI*				m_pRoot = NULL;
	const std::vector<MarkWorkRunningInfor>* m_pvMarkWorkRunInfor = NULL;
	DuiLib::CPaintManagerUI			m_pm;
	HWND							m_hParent;
	bool							m_bBasicModel = true;
	bool							m_pbTypeRun[E_MARK_TYPE_END] = { 0 };
	int								m_pnAchievement[E_MARK_CLASSIFY_END] = { 0 };
	
	TCHAR							m_szPDFFileName[_MAX_PATH] = { 0 };
	TCHAR							m_szCPUName[_MAX_PATH] = { 0 };
	TCHAR							m_szCPUHz[_MAX_PATH] = { 0 };
	TCHAR							m_szCPUNumber[_MAX_PATH] = { 0 };
	TCHAR							m_szPhysicalCPUNumber[_MAX_PATH] = { 0 };
	TCHAR							m_szLogicalCPUNumber[_MAX_PATH] = { 0 };
	TCHAR							m_szMemory[_MAX_PATH] = { 0 };
	TCHAR							m_szGPU[_MAX_PATH] = { 0 };
	TCHAR							m_szProjectNameAndVersion[_MAX_PATH] = { 0 };
	TCHAR							m_szOSNameAndVersion[_MAX_PATH] = { 0 };
	bool							m_bGenerageReportOver = false;
	std::thread						m_threadGenerateReport;
	CWorkMark*						m_pworkMark;
	map<EMarkType, std::tstring>			m_mapItemToName;
	map<EMarkType, int> m_benchmarkMap = {};
};
