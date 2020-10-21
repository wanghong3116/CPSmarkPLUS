#pragma once
#include "Opids.h"
class CWinrar_Opids :
	public COpids
{
public:
	CWinrar_Opids();
	~CWinrar_Opids();
	 
	static int OPID_TYPE_MARKER;
	static int OPID_RAR;
	static int OPID_UNRAR;
	 
	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;

	static tstring DESC_RAR;
	static tstring DESC_UNRAR;
	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;

};

