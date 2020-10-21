#pragma once

class CAntiVirusMark : public CMarkObject
{
public:
	CAntiVirusMark();
	~CAntiVirusMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	TCHAR m_szKingFP[_MAX_PATH]				= { 0 };
	TCHAR m_szDriveCImageFP[_MAX_PATH]		= { 0 };
	TCHAR m_szDriveCCNImageFP[_MAX_PATH] = { 0 };
	TCHAR m_szFindDirImageFP[_MAX_PATH]		= { 0 };

	DWORD dwKismainProcessID = 0;
	HWND hKismainWnd = NULL;

};


