#pragma once
#include "MarkBase.h"
#include "Mark.h"
#include "MarkAuplus.h"

namespace Mark{
	namespace AuWinApi{
		MAPI UInt GetWindowThreadProcessId(Handle);
		MAPI Boolean InProcess(Handle hWnd);
		MAPI POINT PointFromRect(RECT rect, Boolean bCenter);
		MAPI POINT ClientToScreen(Handle hWnd, POINT *lpPoint);
		MAPI Boolean ShowCursor(Boolean fShow);
	};
	namespace AuGui{
		namespace ListView{
			MAPI Void EnsureVisible(Handle hWnd, Int iIndex, Boolean fPartialOK = false);
			MAPI RECT GetItemRectEx(Handle hWnd, Int iIndex, Int iPart);
			MAPI Int GetItemCount(Handle hWnd);
			MAPI Void ClickItem(Handle hWnd, Int iIndex, MouseButton sButton = MouseLeft, Boolean fMove = false, Int iClicks = 1, Int iSpeed = 1);
			MAPI Boolean SetItemSelected(Handle hWnd, Int iIndex, Boolean fSelected = true, Boolean fFocused = false);
			MAPI Boolean GetItemSelected(LPointer hWnd, Int iIndex);
		};
	};
	namespace AuMemory{
		typedef struct tagMemMap{
			Handle hProc;
			UInt Size;
			LPointer Mem;
		}MemMap;
		MAPI Handle  _MemGlobalAlloc(UInt iBytes, UInt iFlags = 0);
		MAPI LPointer _MemGlobalFree(LPointer hMem);
		MAPI LPointer _MemGlobalLock(LPointer hMem);
		MAPI UInt _MemGlobalSize(LPointer hMem);
		MAPI Boolean _MemGlobalUnlock(LPointer hMem);
		MAPI Boolean _MemMoveMemory(LPointer pSource, LPointer pDest, UInt iLength);
		MAPI LPointer _MemVirtualAlloc(LPointer pAddress, UInt iSize, UInt iAllocation, UInt iProtect);
		MAPI LPointer _MemVirtualAllocEx(HANDLE hProcess, LPointer pAddress, UInt iSize, UInt iAllocation, UInt iProtect);
		MAPI Boolean _MemVirtualFree(LPointer pAddress, UInt iSize, UInt iFreeType);
		MAPI Boolean _MemVirtualFreeEx(HANDLE hProcess, LPointer lpVirtualMemory, UInt iSize, UInt dwFreeType);
		MAPI Boolean _MemFree(MemMap *lpMemMap);
		MAPI LPointer _MemInit(MemMap *lpMemMap, Handle hWnd, UInt iSize);
		MAPI Boolean _MemRead(MemMap *lpMemMap, LPointer lpVirtualMemory, LPointer lpBuffer, UInt iBufferSize);
		MAPI Boolean _MemWrite(MemMap *lpMemMap, LPointer lpBuffer, LPointer lpVirtualMemory, UInt iBufferSize);
		MAPI Handle __Mem_OpenProcess(UInt dwDesiredAcces, Boolean bInheritHandle, UInt dwProcessId, Boolean bDebugPriv = false);
	};
	namespace AuUDF{
		MAPI Boolean FileDragDropA(Handle hWnd, Int nFiles, LPCString *lpFiles, Int iXPos, Int iYPos);
		MAPI Boolean FileDragDropW(Handle hWnd, Int nFiles, LPCWString *lpFiles, Int iXPos, Int iYPos);
#if defined(UNICODE) || defined(_UNICODE)
#define FileDragDrop FileDragDropW
		//MAPI Boolean FileDragDrop(Handle hWnd, Int nFiles, LPCTString *lpFiles, Int iXPos, Int iYPos);
#else
#define FileDragDrop FileDragDropA
#endif

		MAPI Boolean MsOfficeActivationWizardSkip(LPCTString lpProcessName, Int msTimeOut = 5000, Int msTimeWait = 10000);
		MAPI MThread *MsOfficeActivationWizardSyncSkip(LPCTString lpProcessName, Int msTimeOut = 5000, Int msTimeWait = 10000);
	};
};
