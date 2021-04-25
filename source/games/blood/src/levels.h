//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT

This file is part of NBlood.

NBlood is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//-------------------------------------------------------------------------
#pragma once
#include "screenjob.h"
#include "common_game.h"
#include "inifile.h"
#include "mapinfo.h"

BEGIN_BLD_NS


enum
{
	kMaxMessages = 32,
	kMaxEpisodes = 7,
	kMaxLevels = 16
};

#pragma pack(push, 1)

enum EGameFlag
{
	GF_AdvanceLevel = 1,
	GF_EndGame = 2,
	// 4 was for playing intro cutscenes but is no longer used.
	GF_PlayCutscene = 8,
};

struct GAMEOPTIONS {
    uint8_t nGameType;
    uint8_t nDifficulty;
    char nMonsterSettings;
    int uGameFlags;
    int uNetGameFlags;
    char nWeaponSettings;
    char nItemSettings;
    char nRespawnSettings;
    char nTeamSettings;
    int nMonsterRespawnTime;
    int nWeaponRespawnTime;
    int nItemRespawnTime;
    int nSpecialRespawnTime;
    int weaponsV10x;
    bool bFriendlyFire;
    bool bKeepKeysOnRespawn;
};

#pragma pack(pop)

enum {
    MUS_INTRO = 0,
    MUS_LOADING = 1,
};

struct EPISODEINFO
{
    int nLevels;
    unsigned int bloodbath : 1;
    unsigned int cutALevel : 4;
    char cutsceneAName[BMAX_PATH];
    char cutsceneBName[BMAX_PATH];
    int cutsceneAWave;
    int cutsceneBWave;
    char cutsceneASound[BMAX_PATH];
    char cutsceneBSound[BMAX_PATH];
};

extern EPISODEINFO gEpisodeInfo[];
extern GAMEOPTIONS gSingleGameOptions;
extern GAMEOPTIONS gGameOptions;
extern int gSkill;
extern char BloodIniFile[];
extern bool bINIOverride;
extern int gEpisodeCount;
extern int gNextLevel;
extern bool gGameStarted;

void levelInitINI(const char *pzIni);
void levelOverrideINI(const char *pzIni);
void levelPlayIntroScene(int nEpisode, CompletionFunc completion);
void levelPlayEndScene(int nEpisode, CompletionFunc completion);
void levelSetupSecret(int nCount);
void levelTriggerSecret(int nSecret);
void CheckSectionAbend(const char *pzSection);
void CheckKeyAbend(const char *pzSection, const char *pzKey);
void levelLoadDefaults(void);
// arg: 0 is normal exit, 1 is secret level
void levelEndLevel(int arg);
void levelTryPlayMusic();

END_BLD_NS
