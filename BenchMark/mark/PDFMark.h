#pragma once

class CPDFMark : public CMarkObject
{
public:
	CPDFMark();
	~CPDFMark();

	virtual int init();
	virtual int run();
	virtual int destroy();

private:
	int convertPPTToPdf();
	int openPdfAndDisplay();
private:
	PowerPoint::_ApplicationPtr		m_app = NULL;
	TCHAR							m_szPPTFP[_MAX_PATH]	= { 0 };
	TCHAR							m_szDstPdfFP[_MAX_PATH] = { 0 };
	Acrobat::CAcroAppPtr			m_pAcroApp = NULL;
	Acrobat::CAcroAVDocPtr			m_pAcroAVDoc = NULL;
};


