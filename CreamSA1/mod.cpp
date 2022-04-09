#include "pch.h"
#include "chrmodels.h"

std::string modpath;

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		modpath = path;
		helperFunctions.ReplaceFile("system\\MILES.pvm", "system\\CREAM_DC.pvm");

		WriteData<1>((void*)0x43FA90, 0xC3); // remove welds

		HookCHRMODELS();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}