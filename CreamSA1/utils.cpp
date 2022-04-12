#include "pch.h"
#include "SADXModLoader.h"
#include "mod.h"
#include "utils.h"

#define MILES_FLY_ 15i8

bool IsTailsFlying(taskwk* twp, playerwk* pwp)
{
	return twp->mode == MILES_FLY_ || (twp->ewp->action.list && twp->ewp->action.list->action.motion == MILES_ACTIONS[17]->motion);
}

void njRotateX_(Angle ang)
{
	if (ang)
		njRotateX(0, (unsigned __int16)ang);
}

void njRotateY_(Angle ang)
{
	if (ang)
		njRotateY(0, (unsigned __int16)ang);
}

void njRotateZ_(Angle ang)
{
	if (ang)
		njRotateZ(0, (unsigned __int16)ang);
}

bool OpenModel(ModelInfo** pInfo, const char* path)
{
	std::string path_ = modpath + "\\models\\" + path;

	auto info = new ModelInfo(path_.c_str());

	if (info->getformat() == ModelFormat_Invalid)
	{
		PrintDebug("[CreamSA1] Failed to load model at %s", path_.c_str());
		delete info;
		return false;
	}
	else
	{
		*pInfo = info;
		return true;
	}
}

bool OpenAnim(AnimationFile** pInfo, const char* path)
{
	std::string path_ = modpath + "\\models\\" + path;

	auto info = new AnimationFile(path_.c_str());

	if (info->getmotion() == nullptr)
	{
		PrintDebug("[CreamSA1] Failed to load animation at %s", path_.c_str());
		delete info;
		return false;
	}
	else
	{
		*pInfo = info;
		return true;
	}
}