#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Heal.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPlayer_Heal::CPlayer_Heal()
{
}

CPlayer_Heal::~CPlayer_Heal()
{
}

void CPlayer_Heal::Initialize(CPlayer* Player)
{
    m_pHost = Player;
    StateID = PlayerStateID::Player_Heal;
}

CPlayerState* CPlayer_Heal::Update(CPlayer* Player, const float& fTimeDelta)
{
     m_fBehaviorTime += fTimeDelta;


     if (m_fBehaviorTime > 0.6f) {
         return m_pHost->Get_State(0);
     }

    return nullptr;
}

void CPlayer_Heal::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
}
