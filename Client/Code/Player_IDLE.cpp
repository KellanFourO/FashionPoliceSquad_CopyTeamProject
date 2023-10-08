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
    StateID = PlayerStateID::Player_IDLE;
}

CPlayerState* CPlayer_IDLE::Update(CPlayer* Player, const float& fTimeDelta)
{
     m_fBehaviorTime += fTimeDelta;
//
//     Player->Key_Input(fTimeDelta);
//
     if (m_pHost->Get_XMove() > 30 && m_fBehaviorTime > 1.f) {
         return m_pHost->Get_State(2);
     }else if (m_pHost->Get_XMove() < -30 && m_fBehaviorTime > 1.f) {
         return m_pHost->Get_State(1);
     }

    return nullptr;
}

void CPlayer_IDLE::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
}
