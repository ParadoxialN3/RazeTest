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

#include <string.h>
#include "build.h"
#include "blood.h"
#include "raze_sound.h"

BEGIN_BLD_NS

class BloodSoundEngine : public RazeSoundEngine
{
	// client specific parts of the sound engine go in this class.
	void CalcPosVel(int type, const void* source, const float pt[3], int channum, int chanflags, FSoundID chanSound, FVector3* pos, FVector3* vel, FSoundChan* channel) override;
	TArray<uint8_t> ReadSound(int lumpnum) override;

public:
	BloodSoundEngine()
	{
		S_Rolloff.RolloffType = ROLLOFF_Doom;
		S_Rolloff.MinDistance = 170;            // these are the numbers I got when uncrunching the original sound code.
		S_Rolloff.MaxDistance = 850;
	}

	void StopChannel(FSoundChan* chan) override
	{
		if (chan && chan->SysChannel != nullptr && !(chan->ChanFlags & CHANF_EVICTED) && chan->SourceType == SOURCE_Actor)
		{
			chan->Source = nullptr;
			chan->SourceType = SOURCE_Unattached;
		}
		SoundEngine::StopChannel(chan);
	}



};

//==========================================================================
//
//
// 
//==========================================================================

TArray<uint8_t> BloodSoundEngine::ReadSound(int lumpnum)
{
	auto wlump = fileSystem.OpenFileReader(lumpnum);
	return wlump.Read();
}

void BloodSoundEngine::CalcPosVel(int type, const void* source, const float pt[3], int channum, int chanflags, FSoundID chanSound, FVector3* pos, FVector3* vel, FSoundChan*)
{
	if (pos != nullptr && type != SOURCE_None)
	{
		FVector3 camera;

		if (gMe && gMe->actor) camera = GetSoundPos(&gMe->actor->spr.pos);
		else camera = { 0, 0, 0 }; // don't crash if there is no player.

		if (vel) vel->Zero();

		if (type == SOURCE_Unattached)
		{
			pos->X = pt[0];
			pos->Y = pt[1];
			pos->Z = pt[2];
		}
		else if (type == SOURCE_Actor)
		{
			assert(source != nullptr);
			auto actor = (DBloodActor*)source;

			// Engine expects velocity in units per second, not units per tic.
			if (vel) *vel = { actor->vel.X * (30 / 65536.f), actor->vel.Z * (-30 / 65536.f), actor->vel.Y * (-30 / 65536.f) };
			*pos = GetSoundPos(&actor->spr.pos);
		}
		else if (type == SOURCE_Ambient)
		{
			*pos = camera; // just to be safe. Ambient sounds are in the world but unpositioned
		}
		if ((chanflags & CHANF_LISTENERZ))
		{
			pos->Y = camera.Y;
		}
	}
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void GameInterface::UpdateSounds()
{
	SoundListener listener;

	if (gMe->actor)
	{
		listener.angle = -gMe->actor->spr.ang * float(BAngRadian); // Build uses a period of 2048.
		listener.velocity.Zero();
		listener.position = GetSoundPos(&gMe->actor->spr.pos);
		listener.valid = true;
	}
	else
	{
		listener.position.Zero();
		listener.valid = false;
	}
	listener.underwater = false;
	// This should probably use a real environment instead of the pitch hacking in S_PlaySound3D.
	// listenactor->waterlevel == 3;
	//assert(primaryLevel->Zones.Size() > listenactor->Sector->ZoneNumber);
	listener.Environment = 0;// primaryLevel->Zones[listenactor->Sector->ZoneNumber].Environment;

	listener.ListenerObject = gMe;
	soundEngine->SetListener(listener);
	soundEngine->UpdateSounds(I_GetTime());
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

FSoundID getSfx(FSoundID soundId, float& attenuation, int& pitch, int& relvol)
{
	auto udata = soundEngine->GetUserData(soundId);
	if (pitch < 0) pitch = udata ? udata[0] : 0x10000;

	if (relvol < 0) relvol = 0;
	else if (relvol == 0) relvol = udata && udata[2] ? udata[2] : 80;
	if (relvol > 255) relvol = 255;
	// Limit the attenuation. More than 2.0 is simply too much.
	attenuation = relvol > 0 ? clamp(80.f / relvol, 0.f, 2.f) : 1.f;
	return soundId;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sfxPlay3DSound(int x, int y, int z, int soundId, sectortype* pSector)
{
	if (!SoundEnabled() || soundId < 0) return;
	auto sid = soundEngine->FindSoundByResID(soundId);
	if (sid == 0) return;

	vec3_t xyz = { x, y, z };
	auto svec = GetSoundPos(&xyz);

	float attenuation;
	int pitch = -1;
	int relvol = 0;
	sid = getSfx(sid, attenuation, pitch, relvol);
	auto sfx = soundEngine->GetSfx(sid);
	EChanFlags flags = CHANF_OVERLAP;
	if (sfx && sfx->LoopStart >= 0) flags |= CHANF_LOOP;

	auto chan = soundEngine->StartSound(SOURCE_Unattached, nullptr, &svec, -1, flags, sid, (0.8f / 80.f) * relvol, attenuation, nullptr, pitch / 65536.f);
	if (chan) chan->UserData = sectnum(pSector);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sfxPlay3DSoundCP(DBloodActor* pActor, int soundId, int playchannel, int playflags, int pitch, int volume)
{
	if (!SoundEnabled() || soundId < 0 || !pActor) return;
	auto sid = soundEngine->FindSoundByResID(soundId);
	if (sid == 0) return;

	auto svec = GetSoundPos(&pActor->spr.pos);

	float attenuation;
	sid = getSfx(sid, attenuation, pitch, volume);
	if (volume == -1) volume = 80;

	if (playchannel >= 0)
	{
		playchannel++;   // This is to make 0 a valid channel value.
		if (soundEngine->EnumerateChannels([=](FSoundChan* chan) -> int
			{
				if (chan->SourceType != SOURCE_Actor) return false; // other source types are not our business.
				if (chan->EntChannel == playchannel && (chan->Source == pActor || (playflags & FX_GlobalChannel) != 0))
				{
					if ((playflags & FX_ChannelMatch) != 0 && chan->EntChannel == playchannel)
						return true;
					if ((playflags & FX_SoundMatch) != 0 && chan->OrgID == sid)
						return true;
					soundEngine->StopChannel(chan);
					return -1;
				}
				return false;
			})) return;

	}

	auto sfx = soundEngine->GetSfx(sid);
	EChanFlags flags = playchannel == -1 ? CHANF_OVERLAP : CHANF_NONE;
	if (sfx && sfx->LoopStart >= 0)
	{
		flags |= CHANF_LOOP;
		flags &= ~CHANF_OVERLAP;
	}

	soundEngine->StartSound(SOURCE_Actor, pActor, &svec, playchannel, flags, sid, volume * (0.8f / 80.f), attenuation, nullptr, pitch / 65536.f);
}

void sfxPlay3DSound(DBloodActor* pActor, int soundId, int a3, int a4)
{
	sfxPlay3DSoundCP(pActor, soundId, a3, a4, -1);
}


//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sfxKill3DSound(DBloodActor* pActor, int a2, int a3)
{
	if (!pActor)
		return;

	if (a2 >= 0) a2++;
	auto sid = soundEngine->FindSoundByResID(a3);
	soundEngine->EnumerateChannels([=](FSoundChan* channel)
		{
			if (channel->SourceType == SOURCE_Actor && channel->Source == pActor && (a2 < 0 || a2 == channel->EntChannel) && (a3 < 0 || sid == channel->OrgID))
			{
				soundEngine->StopChannel(channel);
			}
			return false;
		});
}

void sfxKillAllSounds(void)
{
	if (soundEngine) soundEngine->EnumerateChannels([](FSoundChan* channel)
		{
			if (channel->SourceType == SOURCE_Actor || channel->SourceType == SOURCE_Unattached) soundEngine->StopChannel(channel);
			return false;
		});
}


//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sfxSetReverb(bool toggle)
{
	if (toggle)
	{
		FX_SetReverb(128);
		FX_SetReverbDelay(10);
	}
	else
		FX_SetReverb(0);
}

void sfxSetReverb2(bool toggle)
{
	if (toggle)
	{
		FX_SetReverb(128);
		FX_SetReverbDelay(20);
	}
	else
		FX_SetReverb(0);
}

END_BLD_NS
