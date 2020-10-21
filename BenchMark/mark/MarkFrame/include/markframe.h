#pragma once
#include "Mark.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace std{
#if defined(UNICODE) || defined(_UNICODE)
	typedef wstring tstring;
	typedef wstringstream tstringstream;
#else
	typedef string tstring;
	typedef stringstream tstringstream;
#endif
};

namespace Mark{
	class MCLS MProject;
	class MCLS MController;
	EXPIMP_TEMPLATE template class MCLS MNodeMap<MTStr, LPointer>;
	EXPIMP_TEMPLATE template class MCLS MNodeMap<MTStr, MValue>;
	EXPIMP_TEMPLATE template class MCLS MNodeMap<MTStr, MTStr>;
	EXPIMP_TEMPLATE template class MCLS MNodeMap<MTStr, Double>;
	EXPIMP_TEMPLATE template class MCLS MNodeMap<MTStr, MProject *>;
	EXPIMP_TEMPLATE template class MCLS MNodeMap<MTStr, ILog *>;

	typedef MNodeMap<MTStr, LPointer> MapTStrLPointer;
	typedef MNodeMap<MTStr, MValue> MapTStrValue;
	typedef MNodeMap<MTStr, MTStr> MapTStrTStr;
	typedef MNodeMap<MTStr, Double> MapTStrDouble;
	typedef MNodeMap<MTStr, MProject *> MapTStrProject;
	typedef MNodeMap<MTStr, ILog *> MapTStrLog;

	typedef struct VersionDescription{
		UInt major;
		UInt minor;
		UInt build; // 
		UInt revision;
	}VerDesc;
	typedef struct ProjectDescription{
		LPCTString name;
		LPCTString display;
		LPCTString description;
		UInt       progid;
		VerDesc    version;
		Boolean    selected;
		Boolean    checked;
		MProject  *mproject;
	}ProjDesc;

	class MCLS MController : private MThread{
	protected:
		MapTStrTStr m_ParamatersController;
		MapTStrTStr m_ParamatersShared;
		MapTStrDouble m_Results;
		MapTStrProject m_Projects;
		MapTStrLog m_Logs;
		MTStr m_FileResult;

		friend class MController;
		friend class MProject;
	 
	private:
		enum{
			DoChecking,
			DoTesting,
			DoRelease,
			BeingIdle,
		};
 	};
	class MCLS MProject{
	public:
		enum VersionRevision{
			VersionRevisionDebug   = 1,
			VersionRevisionTrace   = 2,
			VersionRevisionRelease = 3,
		};
	protected:
		MTStr m_Name, m_Display, m_Description, m_ResultDescription;
		ProjDesc m_ProjDesc;

		MapTStrTStr m_ParamatersProject;
		MapTStrTStr m_ParametersShared;
		MapTStrDouble m_ResultsShared;

		MController *m_Controller;

		friend class MController;
		friend class MProject;

	private:
		Int m_Counter;
  		Double m_IdleRate;
	public:
 	};
};
