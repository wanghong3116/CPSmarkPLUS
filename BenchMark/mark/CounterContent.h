#pragma once
#include <string>
 class CCounterContent
{
public:
	CCounterContent(tstring sc, tstring d);
	CCounterContent();
	~CCounterContent();
	// project_id	scenario_id	iteration	retry_count	      timestamp	                                  script	                                       opid	measurement	      description
	//     1	           1	          0	          0	              2019 / 8 / 1 9:44	                  PR_OneNote_02_StartApp.exe	              14000	   186	                  Start OneNote
	//     1	           1	          0	          0	              2019 / 8 / 1 9 : 44	                  PR_OneNote_03_ImportNotebook.exe	  14005	    574	              Open OneNote Package

	//vector<tuple<TCHAR*, TCHAR*, TCHAR*, TCHAR*>, tuple<TCHAR*, TCHAR*,   TCHAR*,  TCHAR*, TCHAR*, TCHAR* >> m_counter_title;
	//vector<tuple<int,         int,          int,         int>,        tuple<time_t,   TCHAR*,   int,         double, TCHAR*>, bool> m_counter_vec;

	tstring m_timestamp;
	ULONGLONG m_starttime;
	tstring m_scriptname;
	ULONGLONG m_measurement;
	tstring m_description;
};

