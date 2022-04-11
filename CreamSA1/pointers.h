#pragma once

// Additional SADX pointers

DataArray(PL_JOIN_VERTEX, miles_jv_list, 0x3C4A610, 30);

FunctionPointer(void, DrawCharacterShadow, (taskwk* twp, shadowwk* swp), 0x49F0B0);
void(__cdecl** NodeCallbackFuncPtr)(NJS_OBJECT* obj) = (decltype(NodeCallbackFuncPtr))0x3AB9908;

static const void* const MilesChangeRunningMotionPtr = (void*)0x459DA0;
static inline void MilesChangeRunningMotion(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
	__asm
	{
		push[twp]
		mov edx, [mwp]
		mov ecx, [pwp]
		call MilesChangeRunningMotionPtr
		add esp, 4
	}
}

static const void* const MilesCheckJumpPtr = (void*)0x45BFE0;
static inline BOOL MilesCheckJump(taskwk* twp, playerwk* pwp)
{
	BOOL result;
	__asm
	{
		mov ecx, [twp]
		mov eax, [pwp]
		call MilesCheckJumpPtr
		mov result, eax
	}
	return result;
}

static const void* const MilesCheckInputPtr = (void*)0x45C100;
static inline BOOL MilesCheckInput(taskwk* twp, motionwk2* mwp, playerwk* pwp)
{
	BOOL result;
	__asm
	{
		push[mwp]
		mov esi, [twp]
		mov edi, [pwp]
		call MilesCheckInputPtr
		mov result, eax
		add esp, 4
	}
	return result;
}