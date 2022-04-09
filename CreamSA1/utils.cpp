#include "pch.h"
#include "SADXModLoader.h"
#include "mod.h"
#include "utils.h"

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