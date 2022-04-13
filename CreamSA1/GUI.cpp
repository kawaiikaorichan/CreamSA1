#include "pch.h"
#include "Trampoline.h"

Trampoline* LoadRegObjTextures_t = nullptr;
Trampoline* FreeRegObjTexlists_t = nullptr;

static NJS_TEXNAME CreamGUI_TEXNAME[12] = {};
static NJS_TEXLIST CreamGUI_TEXLIST = { arrayptrandlengthT(CreamGUI_TEXNAME, int) };

static void __cdecl LoadRegObjTextures_r(int a1)
{
	LoadPVM("CreamGUI", &CreamGUI_TEXLIST);
	((decltype(LoadRegObjTextures_r)*)LoadRegObjTextures_t->Target())(a1);
}

static void __cdecl FreeRegObjTexlists_r()
{
	njReleaseTexture(&CreamGUI_TEXLIST);
	((decltype(FreeRegObjTexlists_r)*)FreeRegObjTexlists_t->Target())();
}

void Icons_Init()
{
	LoadRegObjTextures_t = new Trampoline(0x4212E0, 0x4212E5, LoadRegObjTextures_r, false);
	FreeRegObjTexlists_t = new Trampoline(0x420F40, 0x420F45, FreeRegObjTexlists_r, false);
}