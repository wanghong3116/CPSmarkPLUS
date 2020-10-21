#pragma once

class C3DSMaxWhaleMark : public C3DSMaxMark
{
public:
	C3DSMaxWhaleMark();
	~C3DSMaxWhaleMark();

	virtual int run();
protected:
	virtual int wirteScript();
public:
	TCHAR m_szVideoFP[_MAX_PATH] = { 0 };
};


