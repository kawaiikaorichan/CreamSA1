#include "pch.h"
#include "SADXModLoader.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "utils.h"

static ModelInfo* CREAM_MDL;
static ModelInfo* CURLED_MDL;
static ModelInfo* BALL_MDL;
static ModelInfo* ANKLET_L_MDL;
static ModelInfo* ANKLET_R_MDL;
static ModelInfo* HEAD_MDL;
static ModelInfo* BADGE_MDL;
static ModelInfo* EVENT_MDL;

static void ReplaceTailsModels()
{
	OpenModel(&CREAM_MDL, "Cream.sa1mdl");
	OpenModel(&CURLED_MDL, "CreamCurled.sa1mdl");
	OpenModel(&BALL_MDL, "CreamBall.sa1mdl");
	OpenModel(&ANKLET_L_MDL, "CreamAnkletL.sa1mdl");
	OpenModel(&ANKLET_R_MDL, "CreamAnkletR.sa1mdl");
	OpenModel(&HEAD_MDL, "CreamHead.sa1mdl");
	OpenModel(&BADGE_MDL, "CreamBadge.sa1mdl");
	OpenModel(&EVENT_MDL, "CreamEvent.sa1mdl");

	auto root = CREAM_MDL->getmodel();
	auto ankletl = ANKLET_L_MDL->getmodel();
	auto ankletr = ANKLET_R_MDL->getmodel();

	MILES_OBJECTS[0] = root;
	MILES_OBJECTS[1] = MILES_OBJECTS[0];
	MILES_OBJECTS[2] = CURLED_MDL->getmodel();
	MILES_OBJECTS[3] = BALL_MDL->getmodel();
	MILES_OBJECTS[4] = root->child->child->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[5] = MILES_OBJECTS[4];
	MILES_OBJECTS[6] = root->child->child->sibling->sibling;
	MILES_OBJECTS[7] = MILES_OBJECTS[6];
	MILES_OBJECTS[8] = root->child->child->sibling->sibling->child->child->sibling->child;
	MILES_OBJECTS[9] = root->child->child->sibling->sibling->child->child->sibling->child->sibling;
	MILES_OBJECTS[10] = MILES_OBJECTS[8];
	MILES_OBJECTS[11] = MILES_OBJECTS[9];
	MILES_OBJECTS[12] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[13] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[14] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
	MILES_OBJECTS[15] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
	MILES_OBJECTS[16] = MILES_OBJECTS[12];
	MILES_OBJECTS[17] = MILES_OBJECTS[13];
	MILES_OBJECTS[18] = MILES_OBJECTS[14];
	MILES_OBJECTS[19] = MILES_OBJECTS[15];
	MILES_OBJECTS[20] = root->child->child->sibling->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[21] = root->child->child->sibling->sibling->sibling->child->child->child->sibling;
	MILES_OBJECTS[22] = root->child->child->sibling->sibling->sibling->child->child->child->child->sibling;
	MILES_OBJECTS[23] = root->child->child->sibling->sibling->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[24] = root->child->child->sibling->sibling->sibling->sibling->child->child->child->sibling;
	MILES_OBJECTS[25] = root->child->child->sibling->sibling->sibling->sibling->child->child->child->child->sibling;
	MILES_OBJECTS[26] = root->child->child->sibling->sibling->sibling->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[27] = root->child->child->sibling->sibling->sibling->sibling->sibling->child->child->child->sibling;
	MILES_OBJECTS[28] = root->child->child->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling;
	MILES_OBJECTS[29] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[30] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->sibling;
	MILES_OBJECTS[31] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling;
	MILES_OBJECTS[32] = root->child->child->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	MILES_OBJECTS[33] = root->child->child->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child;
	MILES_OBJECTS[34] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	MILES_OBJECTS[35] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child;
	MILES_OBJECTS[36] = root->child->child->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	MILES_OBJECTS[37] = root->child->child->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child;
	MILES_OBJECTS[38] = root->child->child->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	MILES_OBJECTS[39] = root->child->child->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child;
	MILES_OBJECTS[40] = MILES_OBJECTS[20];
	MILES_OBJECTS[41] = MILES_OBJECTS[21];
	MILES_OBJECTS[42] = MILES_OBJECTS[22];
	MILES_OBJECTS[43] = MILES_OBJECTS[23];
	MILES_OBJECTS[44] = MILES_OBJECTS[24];
	MILES_OBJECTS[45] = MILES_OBJECTS[25];
	MILES_OBJECTS[46] = MILES_OBJECTS[26];
	MILES_OBJECTS[47] = MILES_OBJECTS[27];
	MILES_OBJECTS[48] = MILES_OBJECTS[28];
	MILES_OBJECTS[49] = MILES_OBJECTS[29];
	MILES_OBJECTS[50] = MILES_OBJECTS[30];
	MILES_OBJECTS[51] = MILES_OBJECTS[31];
	MILES_OBJECTS[52] = MILES_OBJECTS[32];
	MILES_OBJECTS[53] = MILES_OBJECTS[33];
	MILES_OBJECTS[54] = MILES_OBJECTS[34];
	MILES_OBJECTS[55] = MILES_OBJECTS[35];
	MILES_OBJECTS[56] = MILES_OBJECTS[36];
	MILES_OBJECTS[57] = MILES_OBJECTS[37];
	MILES_OBJECTS[58] = MILES_OBJECTS[38];
	MILES_OBJECTS[59] = MILES_OBJECTS[39];
	MILES_OBJECTS[60] = ankletl->child;
	MILES_OBJECTS[61] = ankletr->child;
	MILES_OBJECTS[62] = root->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
	MILES_OBJECTS[63] = MILES_OBJECTS[62];
	MILES_OBJECTS[64] = ankletr;
	MILES_OBJECTS[65] = ankletr;
	MILES_OBJECTS[66] = root->child->child->sibling->sibling->child->child;
	MILES_OBJECTS[67] = root->child->child->sibling->sibling->child->child->sibling;
	MILES_OBJECTS[68] = MILES_OBJECTS[66];
	MILES_OBJECTS[69] = MILES_OBJECTS[67];
	MILES_OBJECTS[70] = EVENT_MDL->getmodel();

	MILES_MODELS[0] = MILES_OBJECTS[62]->basicdxmodel;
	MILES_MODELS[1] = MILES_MODELS[0];
	MILES_MODELS[2] = MILES_OBJECTS[4]->basicdxmodel;
	MILES_MODELS[3] = MILES_MODELS[2];
	MILES_MODELS[4] = HEAD_MDL->getmodel()->basicdxmodel;
	MILES_MODELS[5] = MILES_OBJECTS[12]->basicdxmodel;
	MILES_MODELS[6] = MILES_OBJECTS[13]->basicdxmodel;
	MILES_MODELS[14] = BADGE_MDL->getmodel()->basicdxmodel;

	for (int i = 0; i < 112; ++i)
	{
		auto action = MILES_ACTIONS[i];
		if (action) action->object = root;
	}

	MILES_ACTIONS[64]->object = MILES_OBJECTS[2];
	MILES_ACTIONS[65]->object = MILES_OBJECTS[3];
}

void HookCHRMODELS()
{
	ReplaceTailsModels();
}