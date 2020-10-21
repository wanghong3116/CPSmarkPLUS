#pragma once
#include "stdafx.h"
#include <string>
#include <Opids.h>
class CWord_Opids : public COpids
{
public:
	CWord_Opids();
	~CWord_Opids();
	 
	static int OPID_TYPE_MARKER;

	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;

	static int OPID_SET_TITLE;
	static int OPID_SET_CHAR_FORM;
	static int OPID_SET_FONTFAMILY;
	static int OPID_SET_FONT ;
	static int OPID_SET_LEVEL_2_TITLE_FORM ;
	static int OPID_INPUT_TEXT ;
	static int OPID_SET_LEVEL_3_TITLE_FORM;
	static int OPID_INVESTIGATION_RESULT;
	static int OPID_CONCLUSION;
	static int OPID_CONTENT;
	static int OPID_ADD_CHART;
	static int OPID_ADD_PICTURE;
	static int OPID_FILL_CHART;
	static int OPID_SET_LEVEL_1_TITLE_FORM;
	static int OPID_FILL_TABLE;
	static int OPID_CLOSE_DOCUMENT;
	static int OPID_ADD_TABLE;
	static int	OPID_ADD_DOCUMENT;
	 
	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;
	static tstring DESC_ADD_DOCUMENT;
	static tstring DESC_SET_TITLE;
	static tstring DESC_SET_CHAR_FORM;
	static tstring DESC_SET_FONTFAMILY;
	static tstring DESC_SET_FONT;
	static tstring DESC_SET_LEVEL_1_TITLE_FORM;
	static tstring DESC_SET_LEVEL_2_TITLE_FORM;
	static tstring DESC_INPUT_TEXT;
	static tstring DESC_SET_LEVEL_3_TITLE_FORM;
	static tstring DESC_CONCLUSION;
	static tstring DESC_CONTENT;
	static tstring DESC_ADD_CHART;
	static tstring DESC_ADD_PICTURE;
	static tstring DESC_FILL_CHART;
	static tstring DESC_FILL_TABLE;
	static tstring DESC_CLOSE_DOCUMENT;
	static tstring DESC_ADD_TABLE;
};
