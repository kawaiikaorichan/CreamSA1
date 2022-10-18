#include "pch.h"
#include "SADXModLoader.h"
#include "Trampoline.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "utils.h"
#include "pointers.h"
#include "evhead.h"

#define TWP_PNUM(twp) twp->counter.b[0]
#define TWP_CHARA(twp) twp->counter.b[1]

#define MILES_INIT 1i8
#define MILES_STND 1i8
#define MILES_TATK 20i8

enum : __int8
{
	MODE_INIT,
	MODE_NORMAL,
	MODE_ATTACK
};

enum : __int8
{
	ANIM_IDLE,
	ANIM_FLY,
	ANIM_ATTACK
};

Trampoline* MilesTalesPrower_t = nullptr;
Trampoline* MilesChkMode_t     = nullptr;

static ModelInfo* CHEESE_MDL     = nullptr;
static ModelInfo* CHEESEBALL_MDL = nullptr;

static AnimationFile* CHEESE_IDLE_ANM   = nullptr;
static AnimationFile* CHEESE_FLY_ANM    = nullptr;
static AnimationFile* CHEESE_ATTACK_ANM = nullptr;

static NJS_ACTION CHEESE_ACTIONS[3] = {};
static CCL_INFO CHEESE_CCL = { 0, CI_FORM_SPHERE, 0x40, 0x41, 0x400, { 0.0f, 1.5f, 0.0f }, 1.3f, 0.0f, 0.0f, 0.0f, 0, 0, 0 };
static task* cheese_tp[8] = {};
static int ComboTimers[8]{};

static NJS_POINT3 GetPosition(NJS_POINT3* orig, NJS_POINT3* dest, float state) {
	NJS_VECTOR result;
	result.x = (dest->x - orig->x) * state + orig->x;
	result.y = (dest->y - orig->y) * state + orig->y;
	result.z = (dest->z - orig->z) * state + orig->z;
	return result;
}

static void __cdecl CheeseBallDisplay(task* tp)
{
	if (!loop_count)
	{
		auto twp = tp->twp;

		___dsSetPalette(2);
		njSetTexture(&MILES_TEXLIST);
		njPushMatrixEx();
		njTranslateEx(&twp->pos);
		njScale(0, 1.0f + 0.15f * njCos(FrameCounterUnpaused * 1000), 1.0f, 1.0f + 0.15f * njSin(FrameCounterUnpaused * 2000));
		dsDrawModel(CHEESEBALL_MDL->getmodel()->basicdxmodel);
		njPopMatrixEx();
		___dsSetPalette(0);
	}
}

static void __cdecl CheeseBallExec(task* tp)
{
	auto twp = tp->twp;
	auto parent_twp = tp->ptp->twp;

	switch (twp->mode)
	{
	case MODE_INIT:
		tp->disp = CheeseBallDisplay;
		twp->mode = MODE_NORMAL;
		break;
	case MODE_NORMAL:
		// Offset from chao
		NJS_POINT3 v = { 0.0f, 4.25f, -1.0f };
		
		// Calc world position
		njPushMatrix(_nj_unit_matrix_);
		njRotateY_(0xC000 - LOWORD(parent_twp->ang.y));
		njCalcPoint(0, &v, &v);
		njPopMatrixEx();
		njAddVector(&v, &parent_twp->pos);

		// Transform current position to destination
		twp->pos = GetPosition(&twp->pos, &v, 0.5f);

		break;
	}

	tp->disp(tp);
}

static void __cdecl CheeseDestruct(task* tp)
{
	cheese_tp[TWP_PNUM(tp->twp)] = nullptr;
}

static void __cdecl CheeseDisplay(task* tp)
{
	if (!loop_count)
	{
		auto twp = tp->twp;

		___dsSetPalette(2);
		njSetTexture(&MILES_TEXLIST);
		njPushMatrixEx();
		njTranslateEx(&twp->pos);
		njRotateY_(0xC000 - LOWORD(twp->ang.y));
		njScale(0, 0.8f, 0.8f, 0.8f);
		njAction(&CHEESE_ACTIONS[twp->smode], twp->value.f);
		njPopMatrixEx();
		___dsSetPalette(0);

		Shadow(twp, 0.33f);
	}
}

static void CheeseNormal(taskwk* twp, taskwk* ptwp, playerwk* ppwp)
{
	if (ptwp->mode == MILES_TATK)
	{
		twp->mode = MODE_ATTACK;
		return;
	}

	// Offset from player
	NJS_POINT3 v = { -5.5f + (0.25f * njCos(FrameCounterUnpaused * 80)), ppwp->p.height - 1.0f + (0.8f * njSin(FrameCounterUnpaused * 80)), 2.3f + (1.0f * njCos(FrameCounterUnpaused * 180)) };

	// Calc world position
	njPushMatrix(_nj_unit_matrix_);
	njTranslateEx(&ptwp->pos);
	njRotateZ_(ptwp->ang.z);
	njRotateX_(ptwp->ang.x);
	njRotateY_(-LOWORD(ptwp->ang.y));
	njCalcPoint(0, &v, &v);
	njPopMatrixEx();

	auto new_pos = GetPosition(&twp->pos, &v, 0.1f); // Get new position
	
	twp->pos = new_pos; // Set new position

	njSubVector(&new_pos, &v); // Get difference from destination
	auto dist = njScalor(&new_pos); // Get distance from destination

	// If close then idle submode, if away then fly submode
	if (dist < 2.0f)
	{
		twp->smode = IsTailsFlying(ptwp, ppwp) ? ANIM_FLY : ANIM_IDLE;
		twp->ang.y = AdjustAngle(twp->ang.y, ptwp->ang.y, 0x300); // Rotate to player angle
	}
	else
	{
		twp->smode = ANIM_FLY;
		twp->ang.y = 0xC000 - NJM_RAD_ANG(atan2(new_pos.x, new_pos.z)); // Rotate to destination
	}
}

static void CheeseAttack(taskwk* twp, taskwk* ptwp, playerwk* ppwp)
{
	if (ptwp->mode != 20i8)
	{
		twp->mode = MODE_NORMAL;
		twp->timer.l = 0;
		return;
	}

	Angle attack_ang = -(++twp->timer.l * 3000);

	// Get position around circle
	NJS_POINT3 v;
	v.x = njCos(attack_ang) * 10.0f;
	v.y = ppwp->p.height * 0.5f;
	v.z = njSin(attack_ang) * 10.0f;

	// Calc world position
	njPushMatrix(_nj_unit_matrix_);
	njTranslateEx(&ptwp->pos);
	njRotateZ_(ptwp->ang.z);
	njRotateX_(ptwp->ang.x);
	njCalcPoint(0, &v, &v);
	njPopMatrixEx();

	twp->pos = GetPosition(&twp->pos, &v, 0.33f); // Move to target
	twp->ang.y = attack_ang + 0xC000;

	// Move player's damage collision:
	ppwp->free.f[0] = twp->pos.x;
	ppwp->free.f[1] = twp->pos.y;
	ppwp->free.f[2] = twp->pos.z;
	ppwp->free.f[3] = v.x;
	ppwp->free.f[4] = v.y;
	ppwp->free.f[5] = v.z;

	twp->smode = ANIM_ATTACK;
}

static void __cdecl CheeseExec(task* tp)
{
	auto twp = tp->twp;
	auto pnum = TWP_PNUM(twp);
	auto ptwp = playertwp[pnum];
	auto ppwp = playerpwp[pnum];

	if (!ptwp || !ppwp || TWP_CHARA(ptwp) != Characters_Tails)
	{
		FreeTask(tp);
		return;
	}

	switch (twp->mode)
	{
	case MODE_INIT:
		tp->dest = CheeseDestruct;
		tp->disp = CheeseDisplay;
		CreateChildTask(2u, CheeseBallExec, tp);
		CCL_Init(tp, &CHEESE_CCL, 1, 4);
		twp->pos = ptwp->pos;
		twp->mode = MODE_NORMAL;
		break;
	case MODE_NORMAL:
		CheeseNormal(twp, ptwp, ppwp);
		break;
	case MODE_ATTACK:
		CheeseAttack(twp, ptwp, ppwp);
		break;
	}

	// Run animation
	twp->value.f = fmod(twp->value.f + 1.0f, (float)(CHEESE_ACTIONS[twp->smode].motion->nbFrame - 1));
	
	EntryColliList(twp); // Add collision
	LoopTaskC(tp); // Call the ball child task
	tp->disp(tp); // Draw
}

static void CreateCheese(int pnum)
{
	if (!cheese_tp[pnum])
	{
		auto tp = CreateElementalTask(2u, 3, CheeseExec);
		TWP_PNUM(tp->twp) = pnum;

		cheese_tp[pnum] = tp;
	}
}

static void __cdecl MilesTalesPrower_r(task* tp)
{
	auto twp = tp->twp;
	auto co2 = (playerwk*)tp->mwp->work.l;

	if (twp->mode == MILES_INIT)
	{
		CreateCheese(TWP_PNUM(twp));
	}

	DisableEV_HeadOnFrames(tp, twp, co2);

	((decltype(MilesTalesPrower_r)*)MilesTalesPrower_t->Target())(tp);
}

static void MilesChkMode_o(playerwk* pwp, motionwk2* mwp, taskwk* twp)
{
	auto target = MilesChkMode_t->Target();
	__asm
	{
		push[twp]
		mov ecx, [mwp]
		mov eax, [pwp]
		call target
		add esp, 4
	}
}

static void __cdecl MilesChkMode_r(playerwk* pwp, motionwk2* mwp, taskwk* twp)
{
	if (twp->mode == MILES_TATK) // Custom attack code
	{
		auto pnum = TWP_PNUM(twp);

		if (MilesCheckInput(twp, mwp, pwp) || MilesCheckJump(twp, pwp))
		{
			ComboTimers[pnum] = 0;
			return;
		}

		// Set animation:
		if (pwp->spd.x > 0.0f)
			MilesChangeRunningMotion(twp, mwp, pwp);
		else
			pwp->mj.reqaction = 0;

		if (++ComboTimers[pnum] >= 0x2Bu)
		{
			ComboTimers[pnum] = 0;

			// Retry if we have the rhythm badge
			if (pwp->equipment & Upgrades_RhythmBadge)
			{
				if (!(per[pnum]->on & AttackButtons))
				{
					twp->mode = MILES_STND;
				}
			}
			else
			{
				twp->mode = MILES_STND;
			}
		}
	}

	MilesChkMode_o(pwp, mwp, twp);
}

static void __declspec(naked) MilesChkMode_w()
{
	__asm
	{
		push[esp + 04h]
		push ecx
		push eax
		call MilesChkMode_r
		pop eax
		pop ecx
		add esp, 4
		retn
	}
}

void InitCheese()
{
	MilesTalesPrower_t = new Trampoline(0x461700, 0x461707, MilesTalesPrower_r); // Hook Tails exec to add Cheese
	MilesChkMode_t = new Trampoline(0x45E5D0, 0x45E5D5, MilesChkMode_w); // Hook Tails modes to add custom attack

	OpenModel(&CHEESE_MDL, "Cheese.sa1mdl");
	OpenModel(&CHEESEBALL_MDL, "CheeseBall.sa1mdl");

	OpenAnim(&CHEESE_IDLE_ANM, "CheeseIdle.saanim");
	OpenAnim(&CHEESE_FLY_ANM, "CheeseFly.saanim");
	OpenAnim(&CHEESE_ATTACK_ANM, "CheeseAttack.saanim");

	auto root = CHEESE_MDL->getmodel();

	CHEESE_ACTIONS[ANIM_IDLE] = { root, CHEESE_IDLE_ANM->getmotion() };
	CHEESE_ACTIONS[ANIM_FLY] = { root, CHEESE_FLY_ANM->getmotion() };
	CHEESE_ACTIONS[ANIM_ATTACK] = { root, CHEESE_ATTACK_ANM->getmotion() };

	// Remove original attack behaviour:
	WriteData((uint8_t*)0x45B180, 0xC3ui8);
	WriteData((uint8_t*)0x45E280, 0xC3ui8);
	WriteData((uint8_t*)0x45F7E4, 0xEBui8);
	WriteData<9>((void*)0x45D6FB, 0x90ui8);
	//WriteData<5>((void*)0x45D74F, 0x90ui8);
}