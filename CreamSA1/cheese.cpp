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
	MODE_NORMAL
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

NJS_POINT3 GetPosition(NJS_POINT3* orig, NJS_POINT3* dest, float state) {
	NJS_VECTOR result;
	result.x = (dest->x - orig->x) * state + orig->x;
	result.y = (dest->y - orig->y) * state + orig->y;
	result.z = (dest->z - orig->z) * state + orig->z;
	return result;
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
		twp->pos = ptwp->pos;
		twp->mode = MODE_NORMAL;
		break;
	case MODE_NORMAL:
		twp->timer.l += 100;
		float offsetX = 0.25f * njCos(twp->timer.l);
		float offsetZ = 1.0f * njCos(twp->wtimer);
		float offsetY = 0.8f * njSin(twp->timer.l);

		// Calc target position
		njPushMatrix(_nj_unit_matrix_);
		njTranslateEx(&ptwp->pos);
		njRotateZ_(ptwp->ang.z);
		njRotateX_(ptwp->ang.x);
		njRotateY_(-LOWORD(ptwp->ang.y));
		njTranslate(0, -1.5f + offsetX, ppwp->p.height - 1.0f + offsetY, 2.3f + offsetZ);
		njGetTranslation(0, &twp->scl);
		njPopMatrixEx();

		twp->pos = GetPosition(&twp->pos, &twp->scl, 0.1f); // Move to target
		twp->ang.y = AdjustAngle(twp->ang.y, ptwp->ang.y, 0x300); // Rotate to player angle

		// Get animation based on distance from target
		NJS_VECTOR pos = twp->pos;
		njSubVector(&pos, &twp->scl);
		if (njScalor(&pos) > 1.0f)
		{
			twp->smode = ANIM_FLY;
		}
		else
		{
			twp->smode = ANIM_IDLE;
		}

		break;
	}

	twp->value.f = fmod(twp->value.f + 1.0f, (float)(CHEESE_ACTIONS[twp->smode].motion->nbFrame - 1));
	tp->disp(tp);
}

static void CreateCheese(int pnum)
{
	if (!cheese_tp[pnum])
	{
		auto tp = CreateElementalTask(2u, 3, CheeseExec);
		TWP_PNUM(tp->twp) = pnum;

		tp->dest = CheeseDestruct;
		tp->disp = CheeseDisplay;

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