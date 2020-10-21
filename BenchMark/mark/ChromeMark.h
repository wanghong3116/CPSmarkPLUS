#pragma once
#include "AutoItX3_DLL.h"
#include "Mark.h"
#include "MarkFrame.h"
#include "MarkAuplus.h"
#include "MarkAuplusUDF.h"


#pragma comment(lib, "AutoItX3_DLL.lib")
#pragma comment(lib, "MarkFrame.lib")
using namespace Mark;

class ChromeHelper;

class CChromeMark : public CMarkObject
{
public:
	CChromeMark();
	~CChromeMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int ExecuteChrome();
	Void StartUp(ChromeHelper* chromeHelper, int nth);
	void GetToPage(ChromeHelper* chromeHelper, tstring url);
	ChromeHelper* CreateHelper(tstring id);
};

class ChromeHelper {
public:
	std::tstring m_Application;
	std::tstring m_PageAddress;
	std::tstring m_Parameters;
	AuProcess m_Process;
	AuWindow m_Window;
public:
	ChromeHelper() {
		//
	};
	~ChromeHelper() {
		//
	};
public:
	Void setApplication(std::tstring application) {
		m_Application = application;
	};
	Void setPageAddress(std::tstring address) {
		m_PageAddress = address;
	};
	Void setParameters(std::tstring parameters) {
		m_Parameters = parameters;
	};
public:

	Void Terminate() {
		m_Process.Terminate(0);
	};
	Void ShutDown() {
		m_Window.Close();
	};
	Void ScrollDown() {
		AU3_WinActivateByHandle((HWND)(m_Window.GetHandle()));
		AU3_MouseWheel(L"down", 10);
	};
	Void ScrollUp() {
		AU3_WinActivateByHandle((HWND)(m_Window.GetHandle()));
		AU3_MouseWheel(L"up", 10);
	};
	Void Activate() {
		m_Window.Activate();
	};
};
