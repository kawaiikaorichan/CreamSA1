#include "pch.h"
#include "SADXModLoader.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "utils.h"
#include "pointers.h"

#define TWP_PNUM(twp) twp->counter.b[0]
#define MILES_WALK 2i8

enum
{
	ANIM_IDLE,
	ANIM_FLY,
	ANIM_RUN,

	ANIM_COUNT
};

static NJS_MATRIX head_mtx;

static ModelInfo* EARS_MDL     = nullptr;
static AnimationFile* IDLE_ANM = nullptr;
static AnimationFile* FLY_ANM  = nullptr;
static AnimationFile* RUN_ANM  = nullptr;

static NJS_MOTION* EarsMotions[ANIM_COUNT]{};
static constexpr float MotionSpeeds[ANIM_COUNT] = { 0.05f, 0.1f, 0.25f };

static float MotionFrames[8]{};

static void __cdecl CreamCallback(NJS_OBJECT* obj)
{
	if (obj == MILES_OBJECTS[4])
	{
		njSetMatrix(head_mtx, _nj_current_matrix_ptr_);
	}
}

static int GetEarAnimation(taskwk* twp, playerwk* pwp)
{
	if (IsTailsFlying(twp, pwp))
	{
		return ANIM_FLY;
	}
	else if (twp->mode == MILES_WALK && (pwp->mj.reqaction == 12 || pwp->mj.reqaction == 13 || pwp->mj.action == 12 ||pwp->mj.action == 13))
	{
		return ANIM_RUN;
	}
	else
	{
		return ANIM_IDLE;
	}
}

static void DrawEars(taskwk* twp, playerwk* pwp)
{
	if (!(twp->flag & Status_Ball))
	{
		auto& frame = MotionFrames[TWP_PNUM(twp)];

		auto anim_id = GetEarAnimation(twp, pwp);
		auto motion = EarsMotions[anim_id];

		if (!IsGamePaused())
		{
			frame = fmodf(frame + MotionSpeeds[anim_id], (float)motion->nbFrame);
		}
		
		njPushMatrix(head_mtx);
		dsDrawShapeMotion(EARS_MDL->getmodel(), motion, motion, frame);
		njPopMatrixEx();
	}
}

static void __cdecl MilesDisplay_r(task* tp)
{
	auto twp = tp->twp;

	if (dsCheckViewV(&twp->pos, 10.0f))
	{
		auto pwp = reinterpret_cast<playerwk*>(tp->mwp->work.ptr);
		
		if (twp->timer.b[3] & 4)
		{
			pwp->free.f[7] = 0.0f;
			pwp->free.f[6] = 1.0f;
		}

		if ((twp->wtimer & 2) != 0)
		{
			if (IsGamePaused())
				DrawCharacterShadow(twp, &pwp->shadow);
			return;
		}

		auto anim_id = pwp->mj.mtnmode == 2 ? pwp->mj.action : pwp->mj.reqaction;

		// Hide ears from the main model
		MILES_OBJECTS[12]->evalflags |= NJD_EVAL_HIDE;
		MILES_OBJECTS[13]->evalflags |= NJD_EVAL_HIDE;

		___dsSetPalette(2);

		njSetTexture(&MILES_TEXLIST);
		njControl3D_Backup();
		njControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL);
		SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);

		njPushMatrixEx();
		{
			NJS_POINT3 v = { 0.0f, pwp->p.height * 0.5f, 0.0f };

			njPushMatrix(_nj_unit_matrix_);
			{
				njRotateZ_(twp->ang.z);
				njRotateX_(twp->ang.x);
				njRotateY_(-LOWORD(twp->ang.y));
				njCalcVector(0, &v, &v);
			}
			njPopMatrixEx();

			njAddVector(&v, &twp->pos);
			twp->cwp->info->center = v;

			njTranslateEx(&v);
			njRotateZ_(twp->ang.z);
			njRotateX_(twp->ang.x);
			njRotateY_(0x8000 - LOWORD(twp->ang.y));

			if (anim_id == 14 && (twp->flag & (Status_OnColli | Status_Ground)))
			{
				njTranslate(0, 0.0f, -1.0f, 0.0f);
				njRotateZ(0, 0x2000);
				njScale(0, 0.7f, 1.1f, 0.8f);
			}

			*NodeCallbackFuncPtr = CreamCallback;

			if (twp->ewp->action.list)
			{
				njSetZCompare(3);
				DrawEventAction(twp);
				npSetZCompare();
			}
			else if (pwp->mj.mtnmode == 2)
			{
				njAction(pwp->mj.actwkptr, pwp->mj.nframe);
			}
			else
			{
				if (twp->flag & Status_Ball)
				{
					if (!IsGamePaused())
					{
						++twp->btimer;
					}

					if (twp->btimer & 0x11)
					{
						anim_id = 16;
					}
				}

				njAction(pwp->mj.plactptr[anim_id].actptr, pwp->mj.nframe);
			}

			DrawEars(twp, pwp);
		}
		njPopMatrixEx();

		ResetMaterial();
		njControl3D_Restore();
		___dsSetPalette(0);

		if (IsGamePaused())
			DrawCharacterShadow(twp, &pwp->shadow);
	}
}

void InitEars()
{
	OpenModel(&EARS_MDL, "CreamEars.sa1mdl");
	OpenAnim(&IDLE_ANM, "CreamEarsIdle.saanim");
	OpenAnim(&FLY_ANM, "CreamEarsFly.saanim");
	OpenAnim(&RUN_ANM, "CreamEarsRun.saanim");

	EarsMotions[ANIM_IDLE] = IDLE_ANM->getmotion();
	EarsMotions[ANIM_FLY] = FLY_ANM->getmotion();
	EarsMotions[ANIM_RUN] = RUN_ANM->getmotion();

	WriteJump(Tails_Display, MilesDisplay_r);
}