#pragma once
#include "Opids.h"
class CChrome_Opids :
	public COpids
{
public:
	CChrome_Opids();
	~CChrome_Opids();
	static int OPID_TYPE_MARKER;
	static tstring  DESC_TYPE_MARKER;


	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;

	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;

};

