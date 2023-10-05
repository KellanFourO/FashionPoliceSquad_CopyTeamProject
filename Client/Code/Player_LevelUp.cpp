#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_LevelUp.h"
#include "Export_System.h"
#include "Export_Utility.h"


CPlayer_LevelUp::CPlayer_LevelUp()
{
}

CPlayer_LevelUp::~CPlayer_LevelUp()
{
}

void CPlayer_LevelUp::Initialize(CPlayer* Player)
{
    m_pHost = Player;
}

CPlayerState* CPlayer_LevelUp::Update(CPlayer* Player, const float& fDeltaTime)
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

void CPlayer_LevelUp::Release(CPlayer* Player)
{

}
