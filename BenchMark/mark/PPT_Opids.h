#pragma once
#include "Opids.h"
class CPPT_Opids :
	public COpids
{
public:
	CPPT_Opids();
	~CPPT_Opids();
	 
	static int OPID_TYPE_MARKER;

	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;

	static int OPID_ADD_DOCUMENT;

	static int OPID_ADD_PAGE;
	static int OPID_FILL_PAGE;

	static int OPID_ADD_CHART;
	static int OPID_FILL_CHART;

	static int OPID_FILL_TABLE;
	static int OPID_ADD_TABLE;
	static int OPID_CLOSE_DOCUMENT;
	static int OPID_ADD_PICTURE;

	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;

	static tstring DESC_ADD_DOCUMENT;

	static tstring DESC_ADD_PAGE;
	static tstring DESC_FILL_PAGE;

	static tstring DESC_ADD_CHART;
	static tstring DESC_FILL_CHART;

	static tstring DESC_FILL_TABLE;
	static tstring DESC_ADD_TABLE;
	static tstring DESC_CLOSE_DOCUMENT;
	static tstring DESC_ADD_PICTURE;
};

