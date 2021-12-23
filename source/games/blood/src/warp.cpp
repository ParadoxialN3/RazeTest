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
#include "ns.h"	// Must come before everything else!

#include "build.h"

#include "blood.h"

BEGIN_BLD_NS

ZONE gStartZone[8];
#ifdef NOONE_EXTENSIONS
    ZONE gStartZoneTeam1[8];
    ZONE gStartZoneTeam2[8];
    bool gTeamsSpawnUsed = false;
#endif

void validateLinks()
{
    int snum = 0;
    for (auto& sect: sector)
    {
        DCoreActor* upper = sect.upperLink;
        if (upper && !static_cast<DBloodActor*>(upper)->GetOwner())
        {
            Printf(PRINT_HIGH, "Unpartnered upper link in sector %d\n", snum);
            sect.upperLink = nullptr;
        }
        DCoreActor* lower = sect.lowerLink;
        if (lower && !static_cast<DBloodActor*>(lower)->GetOwner())
        {
            Printf(PRINT_HIGH, "Unpartnered lower link in sector %d\n", snum);
            sect.lowerLink = nullptr;
        }
        snum++;
    }
}


void warpInit(TArray<DBloodActor*>& actors)
{
    #ifdef NOONE_EXTENSIONS
    int team1 = 0; int team2 = 0; gTeamsSpawnUsed = false; // increment if team start positions specified.
    #endif

    for(auto actor : actors)
    {
        if (!actor->exists()) continue;
            if (actor->hasX()) {
                XSPRITE *pXSprite = &actor->x();
                switch (actor->spr.type) {
                    case kMarkerSPStart:
                        if (gGameOptions.nGameType < 2 && pXSprite->data1 >= 0 && pXSprite->data1 < kMaxPlayers) {
                            ZONE *pZone = &gStartZone[pXSprite->data1];
                            pZone->x = actor->spr.pos.X;
                            pZone->y = actor->spr.pos.Y;
                            pZone->z = actor->spr.pos.Z;
                            pZone->sector = actor->spr.sector();
                            pZone->ang = actor->spr.ang;
                        }
                        DeleteSprite(actor);
                        break;
                    case kMarkerMPStart:
                        if (pXSprite->data1 >= 0 && pXSprite->data2 < kMaxPlayers) {
                            if (gGameOptions.nGameType >= 2) {
                                // default if BB or teams without data2 specified
                                ZONE* pZone = &gStartZone[pXSprite->data1];
                                pZone->x = actor->spr.pos.X;
                                pZone->y = actor->spr.pos.Y;
                                pZone->z = actor->spr.pos.Z;
                                pZone->sector = actor->spr.sector();
                                pZone->ang = actor->spr.ang;
                            
                                #ifdef NOONE_EXTENSIONS
                                    // fill player spawn position according team of player in TEAMS mode.
                                    if (gModernMap && gGameOptions.nGameType == 3) {
                                        if (pXSprite->data2 == 1) {
                                            pZone = &gStartZoneTeam1[team1];
                                            pZone->x = actor->spr.pos.X;
                                            pZone->y = actor->spr.pos.Y;
                                            pZone->z = actor->spr.pos.Z;
                                            pZone->sector = actor->spr.sector();
                                            pZone->ang = actor->spr.ang;
                                            team1++;

                                        } else if (pXSprite->data2 == 2) {
                                            pZone = &gStartZoneTeam2[team2];
                                            pZone->x = actor->spr.pos.X;
                                            pZone->y = actor->spr.pos.Y;
                                            pZone->z = actor->spr.pos.Z;
                                            pZone->sector = actor->spr.sector();
                                            pZone->ang = actor->spr.ang;
                                            team2++;
                                        }
                                    }
                                #endif

                            }
                            DeleteSprite(actor);
                        }
                        break;
                    case kMarkerUpLink:
                        actor->spr.sector()->upperLink = actor;
                        actor->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
                        actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
                        break;
                    case kMarkerLowLink:
                        actor->spr.sector()->lowerLink = actor;
                        actor->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
                        actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
                        break;
                    case kMarkerUpWater:
                    case kMarkerUpStack:
                    case kMarkerUpGoo:
                        actor->spr.sector()->upperLink = actor;
                        actor->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
                        actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
                        actor->spr.pos.Z = getflorzofslopeptr(actor->spr.sector(), actor->spr.pos.X, actor->spr.pos.Y);
                        break;
                    case kMarkerLowWater:
                    case kMarkerLowStack:
                    case kMarkerLowGoo:
                        actor->spr.sector()->lowerLink = actor;
                        actor->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
                        actor->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
                        actor->spr.pos.Z = getceilzofslopeptr(actor->spr.sector(), actor->spr.pos.X, actor->spr.pos.Y);
                        break;
                }
            }
    }
    
    #ifdef NOONE_EXTENSIONS
    // check if there is enough start positions for teams, if any used
    if (team1 > 0 || team2 > 0) {
        gTeamsSpawnUsed = true;
        if (team1 < kMaxPlayers / 2 || team2 < kMaxPlayers / 2) {
            viewSetSystemMessage("At least 4 spawn positions for each team is recommended.");
            viewSetSystemMessage("Team A positions: %d, Team B positions: %d.", team1, team2);
        }
    }
    #endif

    for(auto& sect: sector)
    {
        auto actor = barrier_cast<DBloodActor*>(sect.upperLink);
        if (actor && actor->hasX())
        {
            XSPRITE *pXSprite = &actor->x();
            int nLink = pXSprite->data1;
            for(auto& sect: sector)
            {
                auto actor2 = barrier_cast<DBloodActor*>(sect.lowerLink);
                if (actor2 && actor2->hasX())
                {
                    if (actor2->xspr.data1 == nLink)
                    {
                        actor->SetOwner(actor2);
                        actor2->SetOwner(actor);
                    }
                }
            }
        }
    }
	validateLinks();
}

int CheckLink(DBloodActor *actor)
{
    auto pSector = actor->spr.sector();
    auto aUpper = barrier_cast<DBloodActor*>(pSector->upperLink);
    auto aLower = barrier_cast<DBloodActor*>(pSector->lowerLink);
    if (aUpper)
    {
        int z;
        if (aUpper->spr.type == kMarkerUpLink)
            z = aUpper->spr.pos.Z;
        else
            z = getflorzofslopeptr(actor->spr.sector(), actor->spr.pos.X, actor->spr.pos.Y);
        if (z <= actor->spr.pos.Z)
        {
            aLower = aUpper->GetOwner();
            assert(aLower);
            assert(aLower->spr.insector());
            ChangeActorSect(actor, aLower->spr.sector());
            actor->spr.pos.X += aLower->spr.pos.X - aUpper->spr.pos.X;
            actor->spr.pos.Y += aLower->spr.pos.Y - aUpper->spr.pos.Y;
            int z2;
            if (aLower->spr.type == kMarkerLowLink)
                z2 = aLower->spr.pos.Z;
            else
                z2 = getceilzofslopeptr(actor->spr.sector(), actor->spr.pos.X, actor->spr.pos.Y);
            actor->spr.pos.Z += z2-z;
            actor->interpolated = false;
            return aUpper->spr.type;
        }
    }
    if (aLower)
    {
        int z;
        if (aLower->spr.type == kMarkerLowLink)
            z = aLower->spr.pos.Z;
        else
            z = getceilzofslopeptr(actor->spr.sector(), actor->spr.pos.X, actor->spr.pos.Y);
        if (z >= actor->spr.pos.Z)
        {
            aUpper = aLower->GetOwner();
            assert(aUpper);
            assert(aUpper->spr.insector());
            ChangeActorSect(actor, aUpper->spr.sector());
            actor->spr.pos.X += aUpper->spr.pos.X - aLower->spr.pos.X;
            actor->spr.pos.Y += aUpper->spr.pos.Y - aLower->spr.pos.Y;
            int z2;
            if (aUpper->spr.type == kMarkerUpLink)
                z2 = aUpper->spr.pos.Z;
            else
                z2 = getflorzofslopeptr(actor->spr.sector(), actor->spr.pos.X, actor->spr.pos.Y);
            actor->spr.pos.Z += z2-z;
            actor->interpolated = false;
            return aLower->spr.type;
        }
    }
    return 0;
}

int CheckLink(int *x, int *y, int *z, sectortype** pSector)
{
    auto aUpper = barrier_cast<DBloodActor*>((*pSector)->upperLink);
    auto aLower = barrier_cast<DBloodActor*>((*pSector)->lowerLink);
    if (aUpper)
    {
        int z1;
        if (aUpper->spr.type == kMarkerUpLink)
            z1 = aUpper->spr.pos.Z;
        else
            z1 = getflorzofslopeptr(*pSector, *x, *y);
        if (z1 <= *z)
        {
            aLower = aUpper->GetOwner();
            assert(aLower);
            assert(aLower->spr.insector());
            *pSector = aLower->spr.sector();
            *x += aLower->spr.pos.X - aUpper->spr.pos.X;
            *y += aLower->spr.pos.Y - aUpper->spr.pos.Y;
            int z2;
            if (aUpper->spr.type == kMarkerLowLink)
                z2 = aLower->spr.pos.Z;
            else
                z2 = getceilzofslopeptr(*pSector, *x, *y);
            *z += z2-z1;
            return aUpper->spr.type;
        }
    }
    if (aLower)
    {
        int z1;
        if (aLower->spr.type == kMarkerLowLink)
            z1 = aLower->spr.pos.Z;
        else
            z1 = getceilzofslopeptr(*pSector, *x, *y);
        if (z1 >= *z)
        {
            aUpper = aLower->GetOwner();
            assert(aUpper);
            *pSector = aUpper->spr.sector();
            *x += aUpper->spr.pos.X - aLower->spr.pos.X;
            *y += aUpper->spr.pos.Y - aLower->spr.pos.Y;
            int z2;
            if (aLower->spr.type == kMarkerUpLink)
                z2 = aUpper->spr.pos.Z;
            else
                z2 = getflorzofslopeptr(*pSector, *x, *y);
            *z += z2-z1;
            return aLower->spr.type;
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

FSerializer& Serialize(FSerializer& arc, const char* keyname, ZONE& w, ZONE* def)
{
	if (arc.BeginObject(keyname))
	{
		arc("x", w.x)
			("y", w.y)
			("z", w.z)
			("sector", w.sector)
			("angle", w.ang)
			.EndObject();
	}
	return arc;
}

void SerializeWarp(FSerializer& arc)
{
	if (arc.BeginObject("warp"))
	{
		arc.Array("startzone", gStartZone, kMaxPlayers)
			.EndObject();
	}
}

END_BLD_NS
