#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Right.h"
#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Player_Walk.h"
//#include "..\Header\Player_Attack.h"

CPlayer_Right::CPlayer_Right()
{
}

CPlayer_Right::~CPlayer_Right()
{
}

void CPlayer_Right::Initialize(CPlayer* Player)
{
    m_pHost = Player;
    StateID = PlayerStateID::Player_Right;
}

CPlayerState* CPlayer_Right::Update(CPlayer* Player, const float& fTimeDelta)
{
	m_fBehaviorTime += fTimeDelta;


     if (m_fBehaviorTime > 0.6f) {
         return m_pHost->Get_State(0);
     }

    return nullptr;
}

void CPlayer_Right::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
}
