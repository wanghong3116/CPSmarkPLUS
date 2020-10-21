
// BenchMarkDlg.h : header file
//

#pragma once
#include "afxwin.h"

class CStartDialogUI;
class CMainTabUI;

// CBenchMarkDlg dialog
class CBenchMarkDlg : public CDialogEx
{
// Construction
public:
	CBenchMarkDlg(CWnd* pParent = NULL);	// standard constructor
	~CBenchMarkDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BENCHMARK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	BOOL m_bRunThread = FALSE;
	CEdit m_editRunInfor;

	int initConfig();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton m_buttonStartUI;
	//CStartDialogUI *m_pStartUI;
	CMainTabUI *m_pMainTabUI;
	
};
