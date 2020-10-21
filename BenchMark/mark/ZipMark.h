#pragma once

class CZipMark : public CMarkObject
{
public:
	CZipMark();
	~CZipMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int WzipDemo7Z();

	TCHAR m_sz7zFP[_MAX_PATH];
	TCHAR m_szSourceFP[_MAX_PATH];
};


