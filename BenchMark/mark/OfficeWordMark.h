#pragma once
#include "Word_Opids.h"
class COfficeWordMark : public CMarkObject
{
public:
	COfficeWordMark();
	~COfficeWordMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
private:
	int WordDemo2();
	MyPlatform::tstring FileRead(const TCHAR* lpFilePath);
	Word::RangePtr FindText(Word::_DocumentPtr lpDoc, const TCHAR* lpText, Word::RangePtr lpLastRange = 0);
	void InsertAfter(Word::RangePtr oRange, MyPlatform::tstring tContent, float ms = 250);
	void Set1stLevelTitleForm(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring Title, std::tstring fontname);
	void Set2ndLevelTitleForm(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring Title, std::tstring fontname);
	void Set3rdLevelTitleForm(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring Title, std::tstring fontname);
	Word::InlineShapePtr AddChart(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, Word::InlineShapePtr wordShape, std::tstring chartname, Office::XlChartType type);
	Word::SelectionPtr AddTable(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, Word::TablesPtr wordTables, Word::TablePtr wordTable, Word::SelectionPtr wordSelection, std::tstring tablename, int nrow, int ncolumn);
	void FillTable(Word::SelectionPtr pwordSelection, CHAR **strs, int nrow, int ncolumn, std::tstring op_name);
	Void FillChart(Word::InlineShapePtr wordShape, CHAR **strs, int nrow, int ncolumn, std::tstring op_name);
	void AddPicture(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring picturename, std::tstring lpPathFile, std::tstring  strRescFP, std::tstring picturepathname);

public:
	Word::_ApplicationPtr	m_pWordApp = NULL;
	CString					m_strRescFP;
	CString					m_strDocFP;
};


