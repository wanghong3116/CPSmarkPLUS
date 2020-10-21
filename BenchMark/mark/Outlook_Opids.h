#pragma once
#include "Opids.h"
class COutlook_Opids :
	public COpids
{
public:
	COutlook_Opids();
	~COutlook_Opids();
	 
	static int OPID_TYPE_MARKER ;
	static int OPID_OPEN_APPLICATION ;
	static int	OPID_OPEN_DOCUMENT ;
	static int OPID_OPEN_MAIL ;
	static int OPID_SEND_MAIL ;
	static int OPID_PREVIEW_ATTACHMENTS ;

	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;
	static tstring DESC_OPEN_MAIL;
	static tstring DESC_SEND_MAIL;
	static tstring DESC_PREVIEW_ATTACHMENTS;
};

