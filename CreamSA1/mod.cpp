#include "pch.h"
#include "SADXModLoader.h"
#include "chrmodels.h"
#include "ears.h"
#include "cheese.h"

std::string modpath;

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		modpath = path;
		helperFunctions.ReplaceFile("system\\MILES.pvm", "system\\CREAM_DC.pvm");

		HookCHRMODELS();
		InitEars();
		InitCheese();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}