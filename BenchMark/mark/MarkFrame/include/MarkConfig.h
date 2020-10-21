#pragma once

#define DEBUG_ALLOCTOR 0
#define _CRT_SECURE_NO_WARNINGS

#include "Mark.h"

#if DEBUG_ALLOCTOR
namespace Mark{
	ILog &GetGlobalDebugHandle();
};
#define LOGHANDLE Mark::GetGlobalDebugHandle()
#endif
