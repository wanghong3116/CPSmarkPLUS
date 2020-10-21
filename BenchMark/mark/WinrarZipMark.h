#pragma once

class CWinrarZipMark : public CMarkObject
{
#define TEST_ZIP_FILE_NAME _T("testWinrarZip.rar")
public:
	CWinrarZipMark();
	~CWinrarZipMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int WzipDemo7Z();
	int copyTestFile();

	int addZipFileByMouse();
	int unZipFileByMose(const TCHAR* szZipFileDir);

	TCHAR m_szWinrarFP[_MAX_PATH];
	TCHAR m_szSourceDir[_MAX_PATH];
};


