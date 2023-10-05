#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Idle.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPlayer_IDLE::CPlayer_IDLE()
{
}

CPlayer_IDLE::~CPlayer_IDLE()
{
}

void CPlayer_IDLE::Initialize(CPlayer* Player)
{
    m_pHost = Player;
}

CPlayerState* CPlayer_IDLE::Update(CPlayer* Player, const float& fDeltaTime)
{
//     m_fBehaviorTime += fDeltaTime;
//
//     Player->Key_Input(fDeltaTime);
//
//     if (Player->m_fXmove > 30) {
//         return new CPlayer_Right;
//     }else if (Player->m_fXmove < -30) {
//         return new CPlayer_Left;
//     }

    return nullptr;
}

void CPlayer_IDLE::Release(CPlayer* Player)
{

}
