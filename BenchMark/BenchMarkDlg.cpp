
// BenchMarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BenchMark.h"
#include "BenchMarkDlg.h"
#include "afxdialogex.h"
#include "MarkObject.h"
#include "AEMark.h"
#include "HandBrake.h"
//#include "OpenCL.h"
#include "Photoshop.h"
#include "VideoPlayer.h"
#include "3DSMax.h"
#include "OfficeWordMark.h"
#include "OfficeExcelMark.h"
#include "OfficePowerPointMark.h"
//#include "ZipMark.h"
#include "ChromeMark.h"
//#include "StartDlgUI.h"
#include "PremiereMark.h"
//#include "MonitorUI.h"
#include "WinrarZipMark.h"
#include "PDFMark.h"
#include "MainTabUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MyPlatform::CIni g_ini;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBenchMarkDlg dialog



CBenchMarkDlg::CBenchMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BENCHMARK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBenchMarkDlg::~CBenchMarkDlg()
{
// 	if (NULL != m_pStartUI)
// 	{
// 		delete m_pStartUI;
// 		m_pStartUI = NULL;
// 	}
}

void CBenchMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCE_BENCHMARK_RUN_INFORMATION, m_editRunInfor);
	DDX_Control(pDX, IDCB_BENCHMARK_UI, m_buttonStartUI);
}

BEGIN_MESSAGE_MAP(CBenchMarkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CBenchMarkDlg message handlers



BOOL CBenchMarkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	initConfig();
	{
		CImage image;
		TCHAR szImagFP[_MAX_PATH] = { 0 };

		MyPlatform::GetFullPathUnderExe(_T("skin\\main\\Main_background.png"), szImagFP);
		image.Load(szImagFP);
		CRect rectWindow, rectClient;
		GetWindowRect(rectWindow);
		GetClientRect(rectClient);
		MoveWindow(CRect(0, 0, image.GetWidth() + rectWindow.Width() - rectClient.Width(), image.GetHeight() + rectWindow.Height() - rectClient.Height()));

	}

	m_buttonStartUI.ShowWindow(SW_HIDE);
	m_pMainTabUI = new CMainTabUI();
	CRect rectClient;
	GetClientRect(rectClient);
	//::SetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA, reinterpret_cast<LPARAM>(m_pMainTabUI));
	m_pMainTabUI->Subclass(GetSafeHwnd());
	m_pMainTabUI->LoadSkin();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBenchMarkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBenchMarkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBenchMarkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CBenchMarkDlg::initConfig()
{
	
	g_ini.SafeString(CFG_MARK, CFG_MARK_3DSMAX_FILE_PATH, _T(""));
	g_ini.SafeInt(CFG_MARK, CFG_MARK_3DSMAX_CAR, 0);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_3DSMAX_HOME, 0);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_3DSMAX_WHALE, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_AE, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_NET, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_OUTLOOK, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_HANDBRAKE, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_OFFICE_EXCEL, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_OFFICE_POWERPOINT, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_OFFICE_WORD, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_OPNCL, 0);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_PDF, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_PHOTOSHOP, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_PREMIERE, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_VIDEOPLAYER, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_WINRAR, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_CAD, 1);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_ANTIVIRUS, 1);

	//g_ini.SafeInt(CFG_BATTERY, CFG_BATTERY_LIFE_PERCENT, 50);
	g_ini.SafeInt(CFG_MARK, CFG_MARK_AE_RENDER_MAX_TIME, 15);


	g_ini.SafeInt(CFG_PHOTOSHOP, CFG_PHOTOSHOP_FILTER_VALUE, 40);
	g_ini.SafeInt(CFG_PHOTOSHOP, CFG_PHOTOSHOP_FILTER_COUNT, 5);

	g_ini.SafeString(CFG_SETTING, CFG_SETTING_PROJECT_NAME, _T("NIM"));
	g_ini.SafeString(CFG_SETTING, CFG_SETTING_PROJECT_VERSION, _T("1.0"));
	MyPlatform::CInformationTrace::IsInTestMode = g_ini.SafeInt(CFG_SETTING, CFG_SETTING_ISINTESTMODE, 0);
	return BENCH_MARK_OK;
}

LRESULT CBenchMarkDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{


	return CDialogEx::WindowProc(message, wParam, lParam);
}
