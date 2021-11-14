#include "ns.h"
#include "wh.h"

BEGIN_WH_NS

static void throwspank(PLAYER& plr, DWHActor* actor);

static void chasefatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0)
		spr.lotag = 250;

	short osectnum = spr.sectnum;

	if (krand() % 63 == 0) {
		if (cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y,
			spr.z - (tileHeight(spr.picnum) << 7), spr.sectnum))// && invisibletime < 0)
			SetNewStatus(actor, ATTACK);
	}
	else {
		checksight(plr, actor);
		if (!checkdist(actor, plr.x, plr.y, plr.z)) {
			auto moveStat = aimove(actor);
			if (moveStat.type == kHitFloor)
			{
				spr.ang = (short)((spr.ang + 1024) & 2047);
				SetNewStatus(actor, FLEE);
				return;
			}
		}
		else {
			if (krand() % 8 == 0) // NEW
				SetNewStatus(actor, ATTACK); // NEW
			else { // NEW
				spr.ang = (short)(((krand() & 512 - 256) + spr.ang + 1024) & 2047); // NEW
				SetNewStatus(actor, FLEE); // NEW
			}
		}
	}

	getzrange(spr.x, spr.y, spr.z - 1, spr.sectnum, (spr.clipdist) << 2, CLIPMASK0);
	spr.z = zr_florz;

	if ((spr.sectnum != osectnum) && (spr.sector()->lotag == 10))
		warpsprite(actor);

	if (checksector6(actor))
		return;

	processfluid(actor, zr_florHit, false);

	if (sector[osectnum].lotag == KILLSECTOR) {
		spr.hitag--;
		if (spr.hitag < 0)
			SetNewStatus(actor, DIE);
	}

	SetActorPos(actor, &spr.pos);
}
	
static void resurectfatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		SetNewStatus(actor, FACE);
		spr.picnum = FATWITCH;
		spr.hitag = (short)adjusthp(90);
		spr.lotag = 100;
		spr.cstat |= 1;
	}
}
	
static void searchfatwitch(PLAYER& plr, DWHActor* actor)
{
	aisearch(plr, actor, false);
	checksector6(actor);
}
	
static void nukedfatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		spr.picnum++;
		spr.lotag = 24;
		if (spr.picnum == FATWITCHCHAR + 4) {
			trailingsmoke(actor,false);
			DeleteActor(actor);
		}
	}
}
	
static void painfatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		spr.picnum = FATWITCH;
		spr.ang = plr.angle.ang.asbuild();
		SetNewStatus(actor, FLEE);
	}

	aimove(actor);
	processfluid(actor, zr_florHit, false);
	SetActorPos(actor, &spr.pos);
}
	
static void facefatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	boolean cansee = ::cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y, spr.z - (tileHeight(spr.picnum) << 7), spr.sectnum);

	if (cansee && plr.invisibletime < 0) {
		spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
		if (plr.shadowtime > 0) {
			spr.ang = (short)(((krand() & 512 - 256) + spr.ang + 1024) & 2047);
			SetNewStatus(actor, FLEE);
		}
		else {
			spr.owner = plr.spritenum;
			SetNewStatus(actor, CHASE);
		}
	}
	else { // get off the wall
		if (spr.owner == plr.spritenum) {
			spr.ang = (short)(((krand() & 512 - 256) + spr.ang) & 2047);
			SetNewStatus(actor, FINDME);
		}
		else if (cansee) SetNewStatus(actor, FLEE);
	}

	if (checkdist(actor, plr.x, plr.y, plr.z))
		SetNewStatus(actor, ATTACK);
}
	
static void attackfatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	getzrange(spr.x, spr.y, spr.z - 1, spr.sectnum, (spr.clipdist) << 2, CLIPMASK0);
	spr.z = zr_florz;

	switch (checkfluid(actor, zr_florHit)) {
	case TYPELAVA:
	case TYPEWATER:
		spr.z += tileHeight(spr.picnum) << 5;
		break;
	}

	SetActorPos(actor, &spr.pos);

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		if (cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y,
			spr.z - (tileHeight(spr.picnum) << 7), spr.sectnum))
			SetNewStatus(actor, CAST);
		else
			SetNewStatus(actor, CHASE);
	}
	else
		spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
}
	
static void fleefatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	short osectnum = spr.sectnum;

	auto moveStat = aimove(actor);
	if (moveStat.type != kHitFloor && moveStat.type != kHitNone) {
		if (moveStat.type == kHitWall) {
			int nWall = moveStat.index;
			int nx = -(wall[wall[nWall].point2].y - wall[nWall].y) >> 4;
			int ny = (wall[wall[nWall].point2].x - wall[nWall].x) >> 4;
			spr.ang = getangle(nx, ny);
		}
		else {
			spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
			SetNewStatus(actor, FACE);
		}
	}
	if (spr.lotag < 0)
		SetNewStatus(actor, FACE);

	if ((spr.sectnum != osectnum) && (spr.sector()->lotag == 10))
		warpsprite(actor);

	if (checksector6(actor))
		return;

	processfluid(actor, zr_florHit, false);

	SetActorPos(actor, &spr.pos);
}
	
static void castfatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		spr.picnum++;
		spr.lotag = 12;
	}

	if (spr.picnum == FATWITCHATTACK + 3) {
		spr.picnum = FATWITCH;
		throwspank(plr, actor);
		SetNewStatus(actor, CHASE);
	}
	checksector6(actor);
}
	
static void diefatwitch(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;

	if (spr.lotag <= 0) {
		spr.picnum++;
		spr.lotag = 20;

		if (spr.picnum == FATWITCHDEAD) {
			if (difficulty == 4)
				SetNewStatus(actor, RESURECT);
			else {
				kills++;
				SetNewStatus(actor, DEAD);
			}
		}
	}
}


static void throwspank(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	auto spawnedactor = InsertActor(spr.sectnum, MISSILE);
	auto& spawned = spawnedactor->s();

	spritesound(S_WITCHTHROW, actor);

	spawned.x = spr.x;
	spawned.y = spr.y;
	spawned.z = spr.sector()->floorz - ((tileHeight(spr.picnum) >> 1) << 8);
	spawned.cstat = 0; // Hitscan does not hit other bullets
	spawned.picnum = FATSPANK;
	spawned.shade = -15;
	spawned.xrepeat = 64;
	spawned.yrepeat = 64;
	spawned.ang = (short)(((getangle(plr.x - spawned.x, plr.y - spawned.y) + (krand() & 15)
		- 8) + 2048) & 2047);
	spawned.xvel = bcos(spawned.ang, -6);
	spawned.yvel = bsin(spawned.ang, -6);
	long discrim = ksqrt((plr.x - spawned.x) * (plr.x - spawned.x) + (plr.y - spawned.y) * (plr.y - spawned.y));
	if (discrim == 0)
		discrim = 1;
	spawned.zvel = (short)(((plr.z + (48 << 8) - spawned.z) << 7) / discrim);
	spawned.owner = actor->GetSpriteIndex();
	spawned.clipdist = 16;
	spawned.lotag = 512;
	spawned.hitag = 0;
	spawned.pal = 0;
	spawned.backuploc();
}

void createFatwitchAI() {
	auto& e = enemy[FATWITCHTYPE];
	e.info.Init(32, 32, 2048, 120, 0, 64, false, 280, 0);
	e.chase = chasefatwitch;
	e.resurect = resurectfatwitch;
	e.search = searchfatwitch;
	e.nuked = nukedfatwitch;
	e.pain = painfatwitch;
	e.face = facefatwitch;
	e.attack = attackfatwitch;
	e.flee = fleefatwitch;
	e.cast = castfatwitch;
	e.die = diefatwitch;
}

void premapFatwitch(DWHActor* actor) {
	SPRITE& spr = actor->s();

	spr.detail = FATWITCHTYPE;
	ChangeActorStat(actor, FACE);
	enemy[FATWITCHTYPE].info.set(spr);
	if (spr.pal == 7)
		spr.hitag = (short)adjusthp(290);
	if (krand() % 100 > 50)
		spr.extra = 1;

}

END_WH_NS
