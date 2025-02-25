//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------
#include "ns.h"

#include "build.h"

#include "gamecontrol.h"

#include "game.h"
#include "tags.h"
#include "names2.h"
#include "network.h"
#include "pal.h"

#include "weapon.h"
#include "menus.h"
#include "printf.h"

BEGIN_SW_NS

static uint8_t tempbuf[576], packbuf[576];

gNET gNet;

#define TIMERUPDATESIZ 32

//InputPacket fsync;

//Local multiplayer variables
// should move this to a local scope of faketimerhandler - do it when able to test

bool CommEnabled = false;
uint8_t CommPlayers = 0;

double smoothratio;

// must start out as 0

void InitNetPlayerOptions(void)
{
//    short pnum;
    PLAYER* pp = Player + myconnectindex;

    strncpy(pp->PlayerName, playername, 31);

    // myconnectindex palette
    pp->TeamColor = gs.NetColor;
    DSWActor* actor = pp->actor;
    if (actor)
    {
        actor->spr.pal = PALETTE_PLAYER0 + pp->TeamColor;
        pp->actor->user.spal = actor->spr.pal;
    }
}

END_SW_NS
