//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2017-2019 Nuke.YKT
Copyright (C) 2020 - Christoph Oelckers

This file is part of Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software, you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY, without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program, if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source, 1996 - Todd Replogle
Prepared for public release, 03/21/2003 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#include "ns.h"
#include "global.h"
#include "names_r.h"

BEGIN_DUKE_NS

void initactorflags_r()
{
	settileflag(TFLAG_WALLSWITCH, {
		HANDPRINTSWITCH,
		HANDPRINTSWITCH + 1,
		ALIENSWITCH,
		ALIENSWITCH + 1,
		MULTISWITCH,
		MULTISWITCH + 1,
		MULTISWITCH + 2,
		MULTISWITCH + 3,
		ACCESSSWITCH,
		ACCESSSWITCH2,
		PULLSWITCH,
		PULLSWITCH + 1,
		HANDSWITCH,
		HANDSWITCH + 1,
		SLOTDOOR,
		SLOTDOOR + 1,
		LIGHTSWITCH,
		LIGHTSWITCH + 1,
		SPACELIGHTSWITCH,
		SPACELIGHTSWITCH + 1,
		SPACEDOORSWITCH,
		SPACEDOORSWITCH + 1,
		FRANKENSTINESWITCH,
		FRANKENSTINESWITCH + 1,
		LIGHTSWITCH2,
		LIGHTSWITCH2 + 1,
		POWERSWITCH1,
		POWERSWITCH1 + 1,
		LOCKSWITCH1,
		LOCKSWITCH1 + 1,
		POWERSWITCH2,
		POWERSWITCH2 + 1,
		DIPSWITCH,
		DIPSWITCH + 1,
		DIPSWITCH2,
		DIPSWITCH2 + 1,
		TECHSWITCH,
		TECHSWITCH + 1,
		DIPSWITCH3,
		DIPSWITCH3 + 1,
		NUKEBUTTON,
		NUKEBUTTON + 1 });
	if (isRRRA()) settileflag(TFLAG_WALLSWITCH, {
		MULTISWITCH2,
		MULTISWITCH2 + 1,
		MULTISWITCH2 + 2,
		MULTISWITCH2 + 3,
		RRTILE8464,
		RRTILE8464 + 1 });

	settileflag(TFLAG_ADULT, {
		FEM10,
		NAKED1,
		FEMMAG1,
		FEMMAG2,
		STATUE,
		STATUEFLASH,
		OOZ,
		WALLBLOOD1,
		WALLBLOOD2,
		WALLBLOOD3,
		WALLBLOOD4,
		WALLBLOOD5,
		SUSHIPLATE1,
		SUSHIPLATE2,
		SUSHIPLATE3,
		SUSHIPLATE4,
		DOLPHIN1,
		DOLPHIN2,
		TOUGHGAL });

	settileflag(TFLAG_ELECTRIC, { HURTRAIL });
	settileflag(TFLAG_CLEARINVENTORY, { HURTRAIL, FLOORSLIME, FLOORPLASMA });
	settileflag(TFLAG_SLIME, { FLOORSLIME, FLOORSLIME + 1, FLOORSLIME + 2 });

	for (auto& fa : gs.actorinfo)
	{
		fa.falladjustz = 24 << 8;
	}
	if (isRRRA())
	{
		gs.actorinfo[HULKBOAT].falladjustz = 12 << 8;
		gs.actorinfo[MINIONBOAT].falladjustz = 3 << 8;
		gs.actorinfo[CHEERBOAT].falladjustz = gs.actorinfo[EMPTYBOAT].falladjustz = 6 << 8;
	}
	gs.actorinfo[DRONE].falladjustz = 0;


	setflag(SFLAG_INTERNAL_BADGUY | SFLAG_KILLCOUNT, {
		BOULDER,
		BOULDER1,
		EGG,
		RAT,
		TORNADO,
		BILLYCOCK,
		BILLYRAY,
		BILLYRAYSTAYPUT,
		BRAYSNIPER,
		DOGRUN,
		LTH,
		HULKJUMP,
		BUBBASTAND,
		HULK,
		HULKSTAYPUT,
		HEN,
		DRONE,
		PIG,
		RECON,
		MINION,
		MINIONSTAYPUT,
		COOT,
		COOTSTAYPUT,
		SHARK,
		VIXEN });

	if (isRRRA()) setflag(SFLAG_INTERNAL_BADGUY | SFLAG_KILLCOUNT, {
		SBSWIPE,
		BIKERB,
		BIKERBV2,
		BIKER,
		MAKEOUT,
		CHEERB,
		CHEER,
		CHEERSTAYPUT,
		COOTPLAY,
		BILLYPLAY,
		MINIONBOAT,
		HULKBOAT,
		CHEERBOAT,
		RABBIT,
		MAMA,
		UFO1_RRRA });
	else setflag(SFLAG_INTERNAL_BADGUY | SFLAG_KILLCOUNT, {
		SBMOVE,
		UFO1_RR,
		UFO2,
		UFO3,
		UFO4,
		UFO5 });

	// Why does the pig count as kill? Let's undo that.
	gs.actorinfo[PIG].flags &= ~SFLAG_KILLCOUNT;
	gs.actorinfo[HEN].flags &= ~SFLAG_KILLCOUNT;

	gs.actorinfo[DRONE].flags |= SFLAG_NOWATERDIP;
	gs.actorinfo[VIXEN].flags |= SFLAG_NOCANSEECHECK;
	if (isRRRA())
	{
		setflag(SFLAG_KILLCOUNT, { ROCK, ROCK2 });	//???
		setflag(SFLAG_NODAMAGEPUSH, { HULK, MAMA, BILLYPLAY, COOTPLAY, MAMACLOUD });
		setflag(SFLAG_NOCANSEECHECK, { COOT, COOTSTAYPUT, BIKERB, BIKERBV2, CHEER, CHEERB,
			CHEERSTAYPUT, MINIONBOAT, HULKBOAT, CHEERBOAT, RABBIT, COOTPLAY, BILLYPLAY, MAKEOUT, MAMA });
	}
	else
	{
		setflag(SFLAG_NODAMAGEPUSH, { HULK, SBMOVE });
	}

	setflag(SFLAG_INVENTORY, {
		FIRSTAID,
		STEROIDS,
		HEATSENSOR,
		BOOTS,
		COWPIE,
		HOLODUKE,
		AIRTANK });

	setflag(SFLAG_HITRADIUSCHECK, {
		STATUEFLASH,
		BOWLINGPIN,
		QUEBALL,
		STRIPEBALL
		});

	setflag(SFLAG_MOVEFTA_CHECKSEE, { VIXEN });
	if (isRRRA())
	{
		setflag(SFLAG_MOVEFTA_CHECKSEE, { COOT, COOTSTAYPUT, BIKER, BIKERB, BIKERBV2, CHEER, CHEERB,
				CHEERSTAYPUT, MINIONBOAT, HULKBOAT, CHEERBOAT, RABBIT, COOTPLAY, BILLYPLAY, MAKEOUT, MAMA });
	}

	setflag(SFLAG_TRIGGER_IFHITSECTOR, { EXPLOSION2, EXPLOSION3 });

	setflag(SFLAG_MOVEFTA_MAKESTANDABLE, {
		RUBBERCAN,
		EXPLODINGBARREL,
		WOODENHORSE,
		HORSEONSIDE,
		CANWITHSOMETHING,
		FIREBARREL,
		FIREVASE,
		NUKEBARREL,
		NUKEBARRELDENTED,
		NUKEBARRELLEAKED
		});

	setflag(SFLAG_NOINTERPOLATE, { CRANEPOLE });
	setflag(SFLAG_FALLINGFLAMMABLE, { BOX });
	setflag(SFLAG_INFLAME, { RADIUSEXPLOSION, RPG, FIRELASER, HYDRENT, HEAVYHBOMB, POWDERKEG, COOLEXPLOSION1, OWHIP, UWHIP });
	if (isRRRA()) setflag(SFLAG_INFLAME, { RPG2 });
	setflag(SFLAG_NOFLOORFIRE, { TREE1, TREE2 });
	setflag(SFLAG_HITRADIUS_FLAG1, { BOX, TREE1, TREE2, TIRE });
	setflag(SFLAG_HITRADIUS_FLAG2, { QUEBALL, STRIPEBALL, BOWLINGPIN, DUKELYINGDEAD });
	setflag(SFLAG_CHECKSLEEP, { RUBBERCAN, EXPLODINGBARREL, WOODENHORSE, HORSEONSIDE, CANWITHSOMETHING, FIREBARREL, NUKEBARREL, NUKEBARRELDENTED, NUKEBARRELLEAKED, EGG });
	setflag(SFLAG_NOTELEPORT, { TRANSPORTERSTAR, TRANSPORTERBEAM, BULLETHOLE, WATERSPLASH2, BURNING, FIRE, MUD });
	setflag(SFLAG_SE24_NOCARRY, { BULLETHOLE, BOLT1, BOLT1 + 1,BOLT1 + 2, BOLT1 + 3, CRANE, CRANE1, CRANE2, BARBROKE });
	setflag(SFLAG_SE24_REMOVE, { BLOODPOOL, PUKE, FOOTPRINTS, FOOTPRINTS2, FOOTPRINTS3 });
	setflag(SFLAG_NOFALLER, { CRACK1, CRACK2, CRACK3, CRACK4 });
	setflag(SFLAG2_EXPLOSIVE, {RPG, RADIUSEXPLOSION, SEENINE, OOZFILTER });
	if (isRRRA()) setflag(SFLAG2_EXPLOSIVE, { RPG2 });
	setflag(SFLAG2_BRIGHTEXPLODE, { SEENINE, OOZFILTER });
	setflag(SFLAG2_DOUBLEDMGTHRUST, { RADIUSEXPLOSION, RPG, HYDRENT, HEAVYHBOMB, SEENINE, OOZFILTER, EXPLODINGBARREL, POWDERKEG });
	if (isRRRA()) setflag(SFLAG2_DOUBLEDMGTHRUST, { RPG2 });
	setflag(SFLAG2_BREAKMIRRORS, { RADIUSEXPLOSION, RPG, HYDRENT, HEAVYHBOMB, SEENINE, OOZFILTER, EXPLODINGBARREL, POWDERKEG });
	if (isRRRA()) setflag(SFLAG2_BREAKMIRRORS, { RPG2 });
	setflag(SFLAG2_CAMERA, { CAMERA1 });

	// Animals were not supposed to have this, but due to a coding bug the logic was unconditional for everything in the game.
	for (auto& ainf : gs.actorinfo)
	{
		ainf.flags |= SFLAG_MOVEFTA_WAKEUPCHECK;
	}

	if (isRRRA())
	{
		setflag(SFLAG_MOVEFTA_CHECKSEEWITHPAL8, { MINION });
	}

	gs.actorinfo[RPG2].flags |= SFLAG_FORCEAUTOAIM;

	gs.weaponsandammosprites[0] = RPGSPRITE;
	gs.weaponsandammosprites[1] = CHAINGUNSPRITE;
	gs.weaponsandammosprites[2] = DEVISTATORAMMO;
	gs.weaponsandammosprites[3] = RPGAMMO;
	gs.weaponsandammosprites[4] = RPGAMMO;
	gs.weaponsandammosprites[5] = COWPIE;
	gs.weaponsandammosprites[6] = SHIELD;
	gs.weaponsandammosprites[7] = FIRSTAID;
	gs.weaponsandammosprites[8] = STEROIDS;
	gs.weaponsandammosprites[9] = RPGAMMO;
	gs.weaponsandammosprites[10] = RPGAMMO;
	gs.weaponsandammosprites[11] = RPGSPRITE;
	gs.weaponsandammosprites[12] = RPGAMMO;
	gs.weaponsandammosprites[13] = FREEZESPRITE;
	gs.weaponsandammosprites[14] = FREEZEAMMO;

	TILE_BOX = BOX;
	TILE_TREE1 = TREE1;
	TILE_TREE2 = TREE2;
	TILE_TIRE = TIRE;
	TILE_CONE = CONE;
	TILE_W_FORCEFIELD = W_FORCEFIELD;
	TILE_SCRAP6 = SCRAP6;
	TILE_APLAYER = APLAYER;
	TILE_DRONE = DRONE;
	TILE_MENUSCREEN = MENUSCREEN;
	TILE_SCREENBORDER = isRRRA()? 7629 : BIGHOLE;
	TILE_VIEWBORDER = VIEWBORDER;
	TILE_APLAYERTOP = APLAYERTOP;
	TILE_CAMCORNER = CAMCORNER;
	TILE_CAMLIGHT = CAMLIGHT;
	TILE_STATIC = STATIC;
	TILE_BOTTOMSTATUSBAR = BOTTOMSTATUSBAR;
	TILE_ATOMICHEALTH = ATOMICHEALTH;
	TILE_JIBS6 = JIBS6;
	TILE_FIRE = FIRE;
	TILE_WATERBUBBLE = WATERBUBBLE;
	TILE_SMALLSMOKE = SMALLSMOKE;
	TILE_BLOODPOOL = BLOODPOOL;
	TILE_FOOTPRINTS = FOOTPRINTS;
	TILE_FOOTPRINTS2 = FOOTPRINTS2;
	TILE_FOOTPRINTS3 = FOOTPRINTS3;
	TILE_FOOTPRINTS4 = FOOTPRINTS4;
	TILE_CLOUDYSKIES = CLOUDYSKIES;
	TILE_ARROW = ARROW;
	TILE_ACCESSSWITCH = ACCESSSWITCH;
	TILE_ACCESSSWITCH2 = ACCESSSWITCH2;
	TILE_GLASSPIECES = GLASSPIECES;
	TILE_BETAVERSION = BETAVERSION;
	TILE_MIRROR = MIRROR;
	TILE_HEN = HEN;
	TILE_MOONSKY1 = MOONSKY1;
	TILE_LA = LA;
	TILE_LOADSCREEN = LOADSCREEN;
	TILE_CROSSHAIR = CROSSHAIR;
	TILE_BIGORBIT1 = BIGORBIT1;
	TILE_EGG = EGG;

	gs.playerheight = PHEIGHT_RR;
}

END_DUKE_NS