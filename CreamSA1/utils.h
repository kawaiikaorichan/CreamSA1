#pragma once

#include "ModelInfo.h"
#include "AnimationFile.h"

bool OpenModel(ModelInfo** info, const char* path);
bool OpenAnim(AnimationFile** pInfo, const char* path);