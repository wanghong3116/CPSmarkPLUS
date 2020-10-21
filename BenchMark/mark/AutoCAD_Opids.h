#pragma once
#include "Opids.h"
class CAutoCAD_Opids :
	public COpids
{
public:
	CAutoCAD_Opids();
	~CAutoCAD_Opids();
	static int OPID_TYPE_MARKER;
	static tstring  DESC_TYPE_MARKER;

	static int OPID_OPEN_APPLICATION ;
	static int	OPID_OPEN_DOCUMENT ;
	static int OPID_DRAWING ;
	static int OPID_SCALING ;

	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;
	static tstring DESC_DRAWING;
	static tstring DESC_SCALING;
};

