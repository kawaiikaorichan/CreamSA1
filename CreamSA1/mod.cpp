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

		WriteData((int*)0x45D74B, 774); // Different sound for attack
		WriteData((int*)0x45C6E9, 1252); // Different voice for dash hoops
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}