#pragma once

class CPhotoshopMark : public CMarkObject
{
public:
	CPhotoshopMark();
	~CPhotoshopMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
private:
	int makeCrayfishPoster();
	int makePeoplePoster();
private:

	Photoshop::_ApplicationPtr m_pApp = NULL;
};


