#pragma once

class CAutoCADMark : public CMarkObject
{
public:
	CAutoCADMark();
	~CAutoCADMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
private:
	static DWORD WINAPI ThreadProcCloseWindow(
		_In_ LPVOID lpParameter
	);

	int RunCADMark();
private:
	AutoCAD::IAcadApplicationPtr m_app = NULL;

	HANDLE m_hThreadClose = NULL;

	TCHAR m_szResourceFP[_MAX_PATH] = { 0 };
};


