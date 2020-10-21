#pragma once
#include "Opids.h"
class CExcel_Opids :
	public COpids
{
public:
	CExcel_Opids();
	~CExcel_Opids();
	 
	static int OPID_TYPE_MARKER ;
	
	static int OPID_OPEN_APPLICATION ;
	static int	OPID_OPEN_DOCUMENT ;

	static int OPID_ADD_DOCUMENT ;

	static int OPID_ADD_CHART ;
	static int OPID_FILL_CHART ;

 	static int OPID_CLOSE_DOCUMENT ;
	static int OPID_SELECT_CHART_RANGE ;
	static int OPID_CALCULATION_TEST ;
	static int OPID_FILL_RANGE ;
	
	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;

	static tstring DESC_ADD_DOCUMENT;
	
	static tstring DESC_ADD_CHART;
	static tstring DESC_FILL_CHART;

 	static tstring DESC_CLOSE_DOCUMENT;
	static tstring DESC_SELECT_CHART_RANGE;
	static tstring DESC_CALCULATION_TEST;
	static tstring DESC_FILL_RANGE;
};

