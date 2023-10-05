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
}

CPlayerState* CPlayer_Hit::Update(CPlayer* Player, const float& fDeltaTime)
{
//     m_fBehaviorTime += fDeltaTime;
//
//     if (m_fBehaviorTime > 0.6f) {
//         return new CPlayer_IDLE;
//     }
    return nullptr;
}

void CPlayer_Hit::Release(CPlayer* Player)
{

}
