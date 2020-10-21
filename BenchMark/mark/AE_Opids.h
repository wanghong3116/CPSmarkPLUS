#pragma once
#include "Opids.h"
class CAE_Opids :
	public COpids
{
public:
	CAE_Opids();
	~CAE_Opids();

	static int OPID_TYPE_MARKER;
	static tstring  DESC_TYPE_MARKER;

	static int OPID_RUN;
	static tstring  DESC_RUN;

	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;

	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;
};