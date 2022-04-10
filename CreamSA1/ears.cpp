#include "pch.h"
#include "SADXModLoader.h"
#include "Trampoline.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "utils.h"

FunctionPointer(void, DrawCharacterShadow, (taskwk* twp, shadowwk* swp), 0x49F0B0);
void(__cdecl** NodeCallbackFuncPtr)(NJS_OBJECT* obj) = (decltype(NodeCallbackFuncPtr))0x3AB9908;

static NJS_MATRIX head_mtx;
static ModelInfo* EARS_MDL;

static void njRotateX_(Angle ang)
{
	if (ang)
		njRotateX(0, (unsigned __int16)ang);
}

static void njRotateY_(Angle ang)
{
	if (ang)
		njRotateY(0, (unsigned __int16)ang);
}

static void njRotateZ_(Angle ang)
{
	if (ang)
		njRotateZ(0, (unsigned __int16)ang);
}

static void __cdecl CreamCallback(NJS_OBJECT* obj)
{
	if (obj == MILES_OBJECTS[4])
	{
		njSetMatrix(head_mtx, _nj_current_matrix_ptr_);
	}
}

static void DrawEars(taskwk* twp, playerwk* pwp)
{
	njPushMatrix(head_mtx);
	dsDrawModel(EARS_MDL->getmodel()->basicdxmodel);
	njPopMatrixEx();
}

static void __cdecl MilesDisplay_r(task* tp);
Trampoline MilesDisplay_t(0x460C40, 0x460C47, MilesDisplay_r);
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
}