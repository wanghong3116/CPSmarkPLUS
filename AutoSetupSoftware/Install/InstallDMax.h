#pragma once

#include "InstallAutodesk.h"

class InstallDMax : public InstallAutodesk
{
public:
	InstallDMax();
	~InstallDMax();

	virtual int Install();
};