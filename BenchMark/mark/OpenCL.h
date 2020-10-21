#pragma once

class COpenCLMark : public CMarkObject
{
public:
	COpenCLMark();
	~COpenCLMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	TCHAR m_szBatFP[_MAX_PATH] = { 0 };
};


