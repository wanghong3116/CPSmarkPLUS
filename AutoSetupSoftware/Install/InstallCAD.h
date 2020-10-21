#pragma once

#include "InstallAutodesk.h"

class InstallCAD : public InstallAutodesk
{
public:
	InstallCAD();
	~InstallCAD();

	virtual int Install();
};