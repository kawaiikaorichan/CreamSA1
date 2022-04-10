#include "pch.h"
#include "SADXModLoader.h"
#include "Trampoline.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "utils.h"

#define TWP_PNUM(twp) twp->counter.b[0]
#define TWP_CHARA(twp) twp->counter.b[1]

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

static ModelInfo* CHEESE_MDL = nullptr;
static ModelInfo* CHEESEBALL_MDL = nullptr;
static AnimationFile* CHEESE_IDLE_ANM = nullptr;
static AnimationFile* CHEESE_FLY_ANM = nullptr;
static AnimationFile* CHEESE_ATTACK_ANM = nullptr;

static NJS_ACTION CHEESE_ACTIONS[3] = {};

static task* cheese_tp[8] = {};

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

		njSetTexture(&MILES_TEXLIST);
		njPushMatrixEx();
		njTranslateEx(&twp->pos);
		dsDrawModel(CHEESEBALL_MDL->getmodel()->basicdxmodel);
		njPopMatrixEx();
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
		NJS_POINT3 v = { 0.0f, 4.25f + (0.15f * njCos(FrameCounterUnpaused * 300)), -1.0f };
		
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

		njSetTexture(&MILES_TEXLIST);
		njPushMatrixEx();
		njTranslateEx(&twp->pos);
		njRotateY_(0xC000 - LOWORD(twp->ang.y));
		njScale(0, 0.8f, 0.8f, 0.8f);
		njAction(&CHEESE_ACTIONS[twp->smode], twp->value.f);
		njPopMatrixEx();
	}
}

static void CheeseNormal(taskwk* twp, taskwk* ptwp, playerwk* ppwp)
{
	if (ptwp->mode == 20i8)
	{
		twp->mode = MODE_ATTACK;
		return;
	}

	// Offset from player
	NJS_POINT3 v = { -1.5f + (0.25f * njCos(FrameCounterUnpaused * 80)), ppwp->p.height - 1.0f + (0.8f * njSin(FrameCounterUnpaused * 80)), 2.3f + (1.0f * njCos(FrameCounterUnpaused * 180)) };

	// Calc world position
	njPushMatrix(_nj_unit_matrix_);
	njTranslateEx(&ptwp->pos);
	njRotateZ_(ptwp->ang.z);
	njRotateX_(ptwp->ang.x);
	njRotateY_(-LOWORD(ptwp->ang.y));
	njCalcPoint(0, &v, &v);
	njPopMatrixEx();

	twp->pos = GetPosition(&twp->pos, &v, 0.1f); // Move to target
	twp->ang.y = AdjustAngle(twp->ang.y, ptwp->ang.y, 0x300); // Rotate to player angle

	// Choose animation based on distance from target
	NJS_VECTOR pos = twp->pos;
	njSubVector(&pos, &v);
	if (njScalor(&pos) > 1.0f)
	{
		twp->smode = ANIM_FLY;
	}
	else
	{
		twp->smode = ANIM_IDLE;
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

	Angle attack_ang = -(++twp->timer.l * 2000);

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
	njRotateY_(-LOWORD(ptwp->ang.y));
	njCalcPoint(0, &v, &v);
	njPopMatrixEx();

	twp->pos = GetPosition(&twp->pos, &v, 0.33f); // Move to target
	twp->ang.y = attack_ang + 0xC000;

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

	if (twp->mode == 0)
	{
		CreateCheese(TWP_PNUM(twp));
	}

	((decltype(MilesTalesPrower_r)*)MilesTalesPrower_t->Target())(tp);
}

void InitCheese()
{
	MilesTalesPrower_t = new Trampoline(0x461700, 0x461707, MilesTalesPrower_r);

	OpenModel(&CHEESE_MDL, "Cheese.sa1mdl");
	OpenModel(&CHEESEBALL_MDL, "CheeseBall.sa1mdl");

	OpenAnim(&CHEESE_IDLE_ANM, "CheeseIdle.saanim");
	OpenAnim(&CHEESE_FLY_ANM, "CheeseFly.saanim");
	OpenAnim(&CHEESE_ATTACK_ANM, "CheeseAttack.saanim");

	auto root = CHEESE_MDL->getmodel();

	CHEESE_ACTIONS[ANIM_IDLE] = { root, CHEESE_IDLE_ANM->getmotion() };
	CHEESE_ACTIONS[ANIM_FLY] = { root, CHEESE_FLY_ANM->getmotion() };
	CHEESE_ACTIONS[ANIM_ATTACK] = { root, CHEESE_ATTACK_ANM->getmotion() };
}