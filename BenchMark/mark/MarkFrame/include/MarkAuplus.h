#pragma once
#include "MarkBase.h"
#include "Mark.h"
#include <windows.h>

namespace Mark{
	enum OsVersion{
		OsWindows2000 = 1,
		OsWindowsXp,
		OsWindowsXp64Bit,
		OsWindowsServer2003,
		OsWindowsServer2003_R2,
		OsWindowsVista,
		OsWindowsServer2008,
		OsWindows7,
		OsWindowsServer2008_R2,
		OsWindowsServer2012,
		OsWindows8
	};

	enum OsProduct{
		OsWindowsUltimate = 1,
		OsWindowsProfessional,
		OsWindowsHome,
		OsWindowsServer,
		OsWindowsEnterprise,
		OsWindowsDatacenter,
	};

	enum OsArch{
		OsArch32Bit = 1,
		OsArch64Bit,
	};

	class MCLS AuCpuUsage{
	public:
		AuCpuUsage();
		~AuCpuUsage();
	public: // Methods
		int Get();
		int Get(LPCTSTR pProcessName);
		int Get(DWORD dwProcessID);
		BOOL EnablePerformaceCounters(BOOL bEnable = TRUE);
	private: // Attributes
		bool			m_bFirstTime;
		LONGLONG		m_lnOldValue ;
		LARGE_INTEGER	m_OldPerfTime100nSec;
	};
	class MCLS AuMacro{
	private:
		MTStr m_Cache;
		LPCTString GetInfoByCSIDL(Int);
	public:
		AuMacro();
		~AuMacro();
	public:
		// Macro Reference - AutoIt Related
		LPCTString ScriptName(); //  Filename of the running script. 
		LPCTString ScriptDir(); //  Directory containing the running script. Only includes a trailing backslash when the script is located in the root of a drive. 
		LPCTString ScriptFullPath(); //  Equivalent to @ScriptDir & "\" & @ScriptName 
		LPCTString WorkingDir(); //  Current/active working directory. Only includes a trailing backslash when the script is located in the root of a drive. 

	public:
		// Macro Reference - Directory
		//    Macros for "All Users" data.
		LPCTString AppDataCommonDir(); // Path to Application Data 
		LPCTString DesktopCommonDir(); //  Path to Desktop 
		LPCTString DocumentsCommonDir(); //  Path to Documents 
		LPCTString FavoritesCommonDir(); //  Path to Favorites 
		LPCTString ProgramsCommonDir(); //  Path to Start Menu's Programs folder 
		LPCTString StartMenuCommonDir(); //  Path to Start Menu folder 
		LPCTString StartupCommonDir(); //  Path to Startup folder 
		//    Macros for Current User data.
		LPCTString AppDataDir(); //  Path to current user's Roaming Application Data 
		LPCTString LocalAppDataDir(); //  Path to current user's Local Application Data 
		LPCTString DesktopDir(); //  Path to current user's Desktop 
		LPCTString MyDocumentsDir(); //  Path to My Documents target 
		LPCTString FavoritesDir(); //  Path to current user's Favorites 
		LPCTString ProgramsDir(); //  Path to current user's Programs (folder on Start Menu) 
		LPCTString StartMenuDir(); //  Path to current user's Start Menu 
		LPCTString StartupDir(); //  current user's Startup folder 
		LPCTString UserProfileDir(); //  Path to current user's Profile folder. 
		//    Other macros for the computer system:
		LPCTString HomeDrive(); //  Drive letter of drive containing current user's home directory. 
		LPCTString HomePath(); //  Directory part of current user's home directory. To get the full path, use in conjunction with @HomeDrive. 
		LPCTString HomeShare(); //  Server and share name containing current user's home directory. 
		LPCTString LogonDNSDomain(); //  Logon DNS Domain. 
		LPCTString LogonDomain(); //  Logon Domain. 
		LPCTString LogonServer(); //  Logon server. 
		LPCTString ProgramFilesDir(); //  Path to Program Files folder 
		LPCTString CommonFilesDir(); //  Path to Common Files folder 
		LPCTString WindowsDir(); //  Path to Windows folder 
		LPCTString SystemDir(); //  Path to the Windows' System (or System32) folder. 
		LPCTString TempDir(); //  Path to the temporary files folder. 
		LPCTString ComSpec(); //  Value of %COMSPEC%, the SPECified secondary COMmand interpreter; primary for command line uses, e.g. Run(@ComSpec & " /k help | more") 

	public:
		UInt CPUArch(); //  Returns "X86" when the CPU is a 32-bit CPU and "X64" when the CPU is 64-bit. 
		UInt KBLayout(); //  Returns code denoting Keyboard Layout. See Appendix for possible values. 
		UInt MUILang(); //  Returns code denoting Multi Language if available (Vista is OK by default). See Appendix for possible values. 
		UInt OSArch(); //  Returns one of the following: "X86", "IA64", "X64" - this is the architecture type of the currently running operating system. 
		UInt OSLang(); //  Returns code denoting OS Language. See Appendix for possible values. 
		UInt OSType(); //  Returns "WIN32_NT" for XP/2003/Vista/2008/Win7/2008R2/Win8/2012/Win8.1/2012R2. 
		UInt OSVersion(); //  Returns one of the following: "WIN_81", "WIN_8", "WIN_7", "WIN_VISTA", "WIN_XP", "WIN_XPe", for Windows servers: "WIN_2012R2", "WIN_2012", "WIN_2008R2", "WIN_2008", "WIN_2003"". 
		UInt OSProduct(); //  Returns the OS product. For example, Ultimate, Professional, Home, Server, Enterprise, Datacenter
		UInt OSBuild(); //  Returns the OS build number. For example, Windows 2003 Server returns 3790 
		UInt OSServicePack(); //  Service pack info in the form of "Service Pack 3". 

	public:
		// Macro Reference - Time And Date
		UInt MSec(); // Milliseconds value of clock. Range is 00 to 999. The update frequency of this value depends on the timer resolution of the hardware and may not update every millisecond. 
		UInt Sec(); //  Seconds value of clock. Range is 00 to 59 
		UInt Min(); //  Minutes value of clock. Range is 00 to 59 
		UInt Hour(); //  Hours value of clock in 24-hour format. Range is 00 to 23 
		UInt MDay(); //  Current day of month. Range is 01 to 31 
		UInt Mon(); //  Current month. Range is 01 to 12 
		UInt Year(); //  Current four-digit year 
		UInt WDay(); //  Numeric day of week. Range is 1 to 7 which corresponds to Sunday through Saturday. 
		UInt YDay(); //  Current day of year. Range is 001 to 366 (or 001 to 365 if not a leap year) 
	};

	class MCLS AuEnv{
	private:
		MTStr m_Cache;
	public:
		AuEnv();
		~AuEnv();
	public:
		LPCTString EnvGet(LPCTString name);
		Boolean EnvSet(LPCTString name, LPCTString value);
	};

	typedef struct tagFDDDriveInfo{
		TChar label[0x20];
		UInt type;
		ULLong total;
		ULLong free;
	}DrvInfo;

	class MCLS AuFDD{
	private:
		MTStr m_Cache;
	public:
		AuFDD();
		~AuFDD();
	public:
		Boolean DirCopy(LPCTString src, LPCTString dst, Boolean overwrite = false);
		Boolean DirCreate(LPCTString path);
		ULLong  DirGetSize(LPCTString path);
		Boolean DirMove(LPCTString src, LPCTString dst, Boolean overwrite = false);
		Boolean DirRemove(LPCTString path);
		Boolean DirExists(LPCTString path);
	public:
		UInt    DriveGetCount();
		Boolean DriveGetInfo(UInt index, DrvInfo *lpDrvInfo);
	public:
		Boolean FileCopy(LPCTString src, LPCTString dst, Boolean overwrite = false);
		ULLong  FileGetSize(LPCTString path);
		Boolean FileMove(LPCTString src, LPCTString dst, Boolean overwrite = false);
		Boolean FileDelete(LPCTString path);
		Boolean FileExists(LPCTString path);
		Boolean FileCreateShortcut(LPCTString file, LPCTString lnk, LPCTString workingdir = 0, LPCTString args = 0, LPCTString desc = 0, LPCTString icon = 0, LPCTString hotkey = 0, Int icon_number = 0, Int state = SW_SHOWNORMAL);

		LPCTString FileOpenDialog(LPCTString title, LPCTString init_dir, LPCTString filter, Int options = 0, LPCTString default_name = 0, LPointer hwnd = 0);
		LPCTString FileSaveDialog(LPCTString title, LPCTString init_dir, LPCTString filter, Int options = 0, LPCTString default_name = 0, LPointer hwnd = 0);
		LPCTString FileSelectFolder(LPCTString dialog_text, LPCTString root_dir, Int flag = 0, LPCTString init_dir = 0, LPointer hwnd = 0);

		LPointer FileOpen(LPCTString path);
		Boolean FileClose(LPointer file);
		ULLong  FileGetPos(LPointer file);
		ULLong  FileSetPos(LPointer file);
		Boolean FileWrite(LPointer file, MValue &val);
		Boolean FileWrite(LPointer file, LPointer ptr, UInt len);
		Boolean FileRead(LPointer file, MValue &val);
		Boolean FileRead(LPointer file, LPointer ptr, UInt len);
	public:
		LPCTString IniRead(LPCTString path, LPCTString section, LPCTString key);
		UInt    IniGetSectionsCount(LPCTString path);
		LPCTString IniGetSection(LPCTString path, UInt index);
		UInt    IniGetKeysCount(LPCTString path, LPCTString section);
		LPCTString IniGetKey(LPCTString path, LPCTString section, UInt index);
		Boolean IniWrite(LPCTString path, LPCTString section, LPCTString key, LPCTString value);
	};
	enum MouseButton{
		MouseLeft,
		MouseRight,
		MouseMiddle,
		MouseMain,
		MouseMenu,
		MousePrimary,
		MouseSecondary,
	};
	class MCLS AuMouse{
	public:
		Boolean Click(Int x, Int y, MouseButton button = MouseLeft, Int clicks = 1, Int speed = 10);
		Boolean GetPos(Int *lpX, Int *lpY);
		Boolean Move(Int *lpX, Int *lpY, Int speed = 10);
		Boolean Wheel(Boolean bDirectionUp = true, Int clicks = 1);
	};
	class MCLS AuNetwork{
	public:
		Boolean InetRead(LPCTString url, Int options = 0);
	};
	class MCLS AuProcess{
	public:
		STARTUPINFO m_StartUpInfo;
		PROCESS_INFORMATION m_ProcessInformation;
		HANDLE m_hChildStd_IN_Rd, m_hChildStd_IN_Wr, m_hChildStd_OUT_Rd, m_hChildStd_OUT_Wr;
	private:
		Void initialize(Boolean bShown, Int iStdFlag = 0);
		Void initialize();
	public:
		Boolean Run(LPCTString program, LPCTString lpWorkingDir, Boolean bShowFlag);
		Boolean Run(LPCTString program, LPCTString parameters, LPCTString lpWorkingDir, Boolean bShowFlag);
		Boolean RunWait(LPCTString program, LPCTString lpWorkingDir, Boolean bShowFlag);
		Boolean Shell(LPCTString program, LPCTString lpWorkingDir, Boolean bShowFlag);
		Boolean ExecuteOpen(LPCTString program, LPCTString lpWorkingDir, Boolean bShowFlag);
		Boolean Wait();
		Boolean Wait(Int ms);
		Boolean Exists();
		Void Terminate(UInt nExitCode);
	public:
		enum {
			ProcStdout = 1,
			ProcStderr = 2,
		};
		Boolean Run(Int iStdFlag, LPCTString program, LPCTString lpWorkingDir, Boolean bShowFlag);
		Boolean Run(Int iStdFlag, LPCTString program, LPCTString parameters, LPCTString lpWorkingDir, Boolean bShowFlag);
		MStr ReadStdOut(Boolean bTotal);
	};
	class MCLS AuReg{
	private:
		MValue m_Cache;
	public:
		AuReg();
		~AuReg();
	public:
		Boolean Delete(LPCTString keyname);
		Boolean Delete(LPCTString keyname, LPCTString valuename);
		Boolean Read  (LPCTString keyname, LPCTString valuename, MValue &value);
		Boolean Write (LPCTString keyname, LPCTString valuename, Const MValue &value);
		Boolean Write (LPCTString keyname, LPCTString valuename, UInt value);
		Boolean Write (LPCTString keyname, LPCTString valuename, LPCTString value);
	};
	class MCLS AuTimer{
	public:
		AuTimer();
		~AuTimer();
	public:
		Void   Sleep(Double ms);
		Double TimerInit();
		Double TimerDiff(Double ms);
	};
	class MCLS AuIni{
	private:
		MTStr m_Cache;
	public:
		AuIni();
		~AuIni();
	public:
		LPCTString Read(LPCTString file, LPCTString section, LPCTString key);
		Void Write(LPCTString file, LPCTString section, LPCTString key, LPCTString val);
		Void Write(LPCTString file, LPCTString section, LPCTString key, Int val);
		Void Write(LPCTString file, LPCTString section, LPCTString key, LLong val);
		Void Write(LPCTString file, LPCTString section, LPCTString key, Float val);
		Void Write(LPCTString file, LPCTString section, LPCTString key, Double val);
	};
	/*
	titleregexp
	title
	classregexp
	class
	pid
	pnameregexp
	actived
	inactived
	visible
	invisible
	enabled
	disabled
	*/
	class MCLS AuWindow{
	private:
		Handle m_Handle;
		MTStr m_Cache;
	private:
		Boolean PrivateAttach(LPCTString lpTitle, LPCTString lpText);
	public:
		enum{
			WindowExists = 1,
			WindowVisible = 2,
			WindowEnabled = 4,
			WindowActive = 8,
			WindowMinimized = 16,
			WindowMaximized = 32,
		};
	public:
		operator Handle();
		operator HWND();
		Boolean operator ==(AuWindow &);
		Boolean operator ==(Handle );
		AuWindow &operator =(AuWindow &);
		AuWindow &operator =(Handle );
	public:
		AuWindow();
		AuWindow(AuWindow &);
		AuWindow(Handle );
		~AuWindow();
	public:
		Boolean IsEnable();
		Boolean IsVisible();
	public:
		Boolean Activate();
		Boolean Active();
		Boolean Close();
		Boolean Exists();
		Boolean Flash();
		Boolean GetCaretPos(Int *lpX, Int *lpY);
		//std::vector<Window> GetChildren();
		Boolean GetClientSize(Int *lpWidth, Int *lpHeight);
		Handle  GetHandle();
		Boolean GetPos(Int *lpX, Int *lpY);
		UInt    GetProcess();
		UInt    GetState();
		LPCTString GetText();
		LPCTString GetTitle();
		Boolean Kill();
		Boolean MenuSelectItem(LPCTString lpMenuText = 0, ...);
		Boolean Move(Int x, Int y);
		Boolean Move(Int x, Int y, Int width, Int height);
		Boolean SetOnTop(Boolean bOnTop = true);
		Boolean SetState(Int iStatus);
		Boolean SetTitle(LPCTString lpTitle);
		Boolean SetTrans(Byte transparency);
		Boolean Wait(Int ms = -1);
		Boolean WaitActive(Int ms = -1);
		Boolean WaitClose(Int ms = -1);
		Boolean WaitNotActive(Int ms = -1);
	public:
		Boolean Activate(LPCTString lpTitle, LPCTString lpText);
		Boolean Active(LPCTString lpTitle, LPCTString lpText);
		Boolean Close(LPCTString lpTitle, LPCTString lpText);
		Boolean Exists(LPCTString lpTitle, LPCTString lpText);
		Boolean Flash(LPCTString lpTitle, LPCTString lpText);
		Boolean GetCaretPos(LPCTString lpTitle, LPCTString lpText, Int *lpX, Int *lpY);
		//std::vector<Window> GetChildren(LPCTString lpTitle, LPCTString lpText);
		Boolean GetClientSize(LPCTString lpTitle, LPCTString lpText, Int *lpWidth, Int *lpHeight);
		Handle  GetHandle(LPCTString lpTitle, LPCTString lpText);
		Boolean GetPos(LPCTString lpTitle, LPCTString lpText, Int *lpX, Int *lpY);
		UInt    GetProcess(LPCTString lpTitle, LPCTString lpText);
		UInt    GetState(LPCTString lpTitle, LPCTString lpText);
		LPCTString GetText(LPCTString lpTitle, LPCTString lpText);
		LPCTString GetTitle(LPCTString lpTitle, LPCTString lpText);
		Boolean Kill(LPCTString lpTitle, LPCTString lpText);
		Boolean MenuSelectItem(LPCTString lpTitle, LPCTString lpText, LPCTString lpMenuText = 0, ...);
		Boolean Move(LPCTString lpTitle, LPCTString lpText, Int x, Int y);
		Boolean Move(LPCTString lpTitle, LPCTString lpText, Int x, Int y, Int width, Int height);
		Boolean SetOnTop(LPCTString lpTitle, LPCTString lpText, Boolean bOnTop = true);
		Boolean SetState(LPCTString lpTitle, LPCTString lpText, Int iStatus);
		Boolean SetTitle(LPCTString lpTitle, LPCTString lpText, LPCTString lpNewTitle);
		Boolean SetTrans(LPCTString lpTitle, LPCTString lpText, Byte transparency);
		Boolean Wait(LPCTString lpTitle, LPCTString lpText, Int ms = -1);
		Boolean WaitActive(LPCTString lpTitle, LPCTString lpText, Int ms = -1);
		Boolean WaitClose(LPCTString lpTitle, LPCTString lpText, Int ms = -1);
		Boolean WaitNotActive(LPCTString lpTitle, LPCTString lpText, Int ms = -1);
	public:
		//static std::vector<Window> List(LPCTString lpTitle, LPCTString lpText);
		//static Boolean MinimizeAll(Boolean bUndo = false);
	};
};
