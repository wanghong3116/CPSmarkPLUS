#pragma once

class CAEMark : public CMarkObject
{
public:
	CAEMark();
	~CAEMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int writeScript();
	int writeUTF8File(const TCHAR* szFilePath, const TCHAR* szText);
	int startAEAndCloseRegister();
	int copyPlugin();

	int RunAEMark();

	int runOpenProject();
	int runStarRender();
	int runWaitRenderOver();
private:
	TCHAR m_szAEFP[_MAX_PATH] = { 0 };
	TCHAR m_szScriptOpenFP[_MAX_PATH] = { 0 };
	TCHAR m_szDstVideoFP[_MAX_PATH] = { 0 };
	TCHAR m_szProjectFP[_MAX_PATH * 2] = { 0 };

	HWND m_hAEMainWnd = NULL;
};


