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
}

CPlayerState* CPlayer_Armor::Update(CPlayer* Player, const float& fDeltaTime)
{
//     m_fBehaviorTime += fDeltaTime;
//
//     Player->Key_Input(fDeltaTime);
//
//     if (m_fBehaviorTime > 0.6f) {
//         return new CPlayer_IDLE;
//     }

    return nullptr;
}

void CPlayer_Armor::Release(CPlayer* Player)
{

}
#include "stdafx.h"
#include "Player_Armor.h"
