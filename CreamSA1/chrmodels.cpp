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

DataArray(PL_JOIN_VERTEX, miles_jv_list, 0x3C4A610, 30);

#define INIT_WELD(id, base, mdlA, mdlB, table) miles_jv_list[id] = { MILES_OBJECTS[base], MILES_OBJECTS[mdlA], MILES_OBJECTS[mdlB], static_cast<char>(LengthOfArray(table) / 2), PL_JOIN_SRC, 0, 0, nullptr, (uint16_t*)table }

static const uint16_t Cream_UpperArmIndices_DC[] = {
	0, 2,
	1, 3,
	4, 6,
	5, 7,
};

static const uint16_t Cream_LowerArmIndices_DC[] = {
	0, 10,
	1, 11,
	4, 14,
	5, 15,
};

static const uint16_t Cream_LegIndices_DC[] = {
	0, 2,
	1, 3,
	4, 6,
	5, 7,
};

static const uint16_t Cream_ShoeIndices_DC[] = {
	2, 3,
	12, 8,
	0, 1,
	1, 0,
	17, 13,
	3, 2,
};

static const uint16_t Cream_HandIndices_DC[] = {
	4, 14,
	0, 15,
	1, 13,
	5, 12,
};

static void __cdecl InitTailsWeldInfo_r()
{
	INIT_WELD(0, 0, 20, 21, Cream_UpperArmIndices_DC);
	INIT_WELD(1, 0, 21, 22, Cream_LowerArmIndices_DC);
	INIT_WELD(2, 0, 23, 24, Cream_UpperArmIndices_DC);
	INIT_WELD(3, 0, 24, 25, Cream_LowerArmIndices_DC);
	INIT_WELD(4, 0, 26, 27, Cream_LegIndices_DC);
	INIT_WELD(5, 0, 27, 28, Cream_LegIndices_DC);
	INIT_WELD(6, 0, 29, 30, Cream_LegIndices_DC);
	INIT_WELD(7, 0, 30, 31, Cream_LegIndices_DC);
	INIT_WELD(8, 0, 32, 33, Cream_ShoeIndices_DC);
	INIT_WELD(9, 0, 34, 35, Cream_ShoeIndices_DC);
	INIT_WELD(10, 0, 36, 37, Cream_HandIndices_DC);
	INIT_WELD(11, 0, 38, 39, Cream_HandIndices_DC);

	miles_jv_list[12] = { MILES_OBJECTS[0], MILES_OBJECTS[37], nullptr, 2, PL_JOIN_RHAND_POS, 0, 0, nullptr, nullptr };
	miles_jv_list[13] = { MILES_OBJECTS[0], MILES_OBJECTS[39], nullptr, 2, PL_JOIN_LHAND_POS, 0, 0, nullptr, nullptr };
	miles_jv_list[14] = { MILES_OBJECTS[0], MILES_OBJECTS[33], nullptr, 0, PL_JOIN_RFOOT_POS, 0, 0, nullptr, nullptr };
	miles_jv_list[15] = { MILES_OBJECTS[0], MILES_OBJECTS[35], nullptr, 0, PL_JOIN_LFOOT_POS, 0, 0, nullptr, nullptr };
	miles_jv_list[16] = { MILES_OBJECTS[0], MILES_OBJECTS[4], nullptr, 0, PL_JOIN_USER0_POS, 0, 0, nullptr, nullptr };
	miles_jv_list[17] = {};
}

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
	WriteJump(InitTailsWeldInfo, InitTailsWeldInfo_r);
}