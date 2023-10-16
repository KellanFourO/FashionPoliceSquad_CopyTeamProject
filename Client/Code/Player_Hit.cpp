#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Hit.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPlayer_Hit::CPlayer_Hit()
{
}

CPlayer_Hit::~CPlayer_Hit()
{
}

void CPlayer_Hit::Initialize(CPlayer* Player)
{
    m_pHost = Player;
    StateID = PlayerStateID::Player_Hit;
    SoundMgr()->PlaySoundW(L"Player_Hit.wav",SOUND_PLAYER2,1.f);
}

CPlayerState* CPlayer_Hit::Update(CPlayer* Player, const float& fTimeDelta)
{
     m_fBehaviorTime += fTimeDelta;

     if (m_fBehaviorTime > 0.6f) {
         return m_pHost->Get_State(0);
     }
    return nullptr;
}

void CPlayer_Hit::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
    SoundMgr()->StopSound(SOUND_PLAYER2);
}
