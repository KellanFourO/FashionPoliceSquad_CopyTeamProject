#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Armor.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPlayer_Armor::CPlayer_Armor()
{
}

CPlayer_Armor::~CPlayer_Armor()
{
}

void CPlayer_Armor::Initialize(CPlayer* Player)
{
    m_pHost = Player;
    StateID = PlayerStateID::Player_Armor;
}

CPlayerState* CPlayer_Armor::Update(CPlayer* Player, const float& fTimeDelta)
{
     m_fBehaviorTime += fTimeDelta;


     if (m_fBehaviorTime > 0.6f) {
         return m_pHost->Get_State(0);
     }

    return nullptr;
}

void CPlayer_Armor::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
}

