#pragma once
#include "Opids.h"
class CPDF_Opids :
	public COpids
{
public:
	CPDF_Opids();
	~CPDF_Opids();
	 
	static int OPID_TYPE_MARKER;
	static int OPID_OPEN_APPLICATION_ACROBAT;
	static int OPID_OPEN_DOCUMENT_ACROBAT;
	static int OPID_SAVE_AS_PDF_DOCUMENT;
	static int OPID_OPEN_APPLICATION;
	static int	OPID_OPEN_DOCUMENT;

	static tstring  DESC_TYPE_MARKER;
	static tstring DESC_OPEN_APPLICATION_ACROBAT;
	static tstring DESC_OPEN_DOCUMENT_ACROBAT;
	static tstring DESC_OPEN_APPLICATION;
	static tstring DESC_OPEN_DOCUMENT;
	static tstring DESC_SAVE_AS_PDF_DOCUMENT;
};

