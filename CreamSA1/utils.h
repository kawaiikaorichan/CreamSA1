#pragma once

#include "ModelInfo.h"
#include "AnimationFile.h"

void njRotateX_(Angle ang);
void njRotateY_(Angle ang);
void njRotateZ_(Angle ang);

bool OpenModel(ModelInfo** info, const char* path);
bool OpenAnim(AnimationFile** pInfo, const char* path);