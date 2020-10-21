#pragma once

class COfficePowerPointMark : public CMarkObject
{
public:
	COfficePowerPointMark();
	~COfficePowerPointMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int PPTDemo2();

	void FillPage(PowerPoint::_SlidePtr oSlide, CHAR **lpTexts, std::tstring Title);
	void FillChart(PowerPoint::ShapePtr oShape, CHAR **strs, int nrow, int ncolumn, long oLayout, std::tstring Title);
	PowerPoint::ShapePtr AddChart(PowerPoint::_SlidePtr oSlide, Office::XlChartType chartType, float top, float bottom, float left, float right, std::tstring Title);
	PowerPoint::ShapePtr  AddTable(PowerPoint::_SlidePtr oSlide, int nrow, int ncolumn, float top, float bottom, float left, float right, std::tstring Title);
	void FillTable(PowerPoint::ShapePtr pptShape, CHAR **strs, int nrow, int ncolumn, std::tstring Title);
	PowerPoint::_SlidePtr AddPage(int nPage, PowerPoint::_PresentationPtr oPresentation, std::tstring title);
	PowerPoint::ShapePtr AddPicture(PowerPoint::_SlidePtr oSlide, std::tstring lpPathFile, Office::MsoTriState state1, Office::MsoTriState state2, float top, float bottom, float left, float right, std::tstring Title);

private:
	PowerPoint::_ApplicationPtr g_ApplicationPtr;
};


