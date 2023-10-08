#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Left.h"
#include "Export_System.h"
#include "Export_Utility.h"


CPlayer_Left::CPlayer_Left()
{
}

CPlayer_Left::~CPlayer_Left()
{
}

void CPlayer_Left::Initialize(CPlayer* Player)
{
    m_pHost = Player;
    StateID = PlayerStateID::Player_Left;
}

CPlayerState* CPlayer_Left::Update(CPlayer* Player, const float& fTimeDelta)
{
     m_fBehaviorTime += fTimeDelta;

     if (m_fBehaviorTime > 0.6f) {
         return m_pHost->Get_State(0);
     }

    return nullptr;
}

void CPlayer_Left::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
}
