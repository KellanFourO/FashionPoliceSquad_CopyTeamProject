#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_IDLE.h"
#include "Player_Right.h"
#include "Player_Left.h"
#include "Player_Hit.h"
#include "Player_Lv_Up.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Player_Walk.h"
//#include "..\Header\Player_Attack.h"

CPlayer_IDLE::CPlayer_IDLE()
{
}

CPlayer_IDLE::~CPlayer_IDLE()
{
}

void CPlayer_IDLE::Initialize(CPlayer* Player)
{
    StateID = PlayerStateID::Player_IDLE;
}

CPlayerState* CPlayer_IDLE::Update(CPlayer* Player, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    Player->Key_Input(fDeltaTime);
    
    if (Player->m_fXmove > 30) {
        return new CPlayer_Right;
    }else if (Player->m_fXmove < -30) {
        return new CPlayer_Left;
    }

    return nullptr;
}

void CPlayer_IDLE::Release(CPlayer* Player)
{
    
}
