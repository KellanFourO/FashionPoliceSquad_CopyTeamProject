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

CPlayer_Hit::CPlayer_Hit()
{
}

CPlayer_Hit::~CPlayer_Hit()
{
}

void CPlayer_Hit::Initialize(CPlayer* Player)
{
    StateID = PlayerStateID::Player_Hit;
}

CPlayerState* CPlayer_Hit::Update(CPlayer* Player, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime > 0.6f) {
        return new CPlayer_IDLE;
    }



    return nullptr;
}

void CPlayer_Hit::Release(CPlayer* Player)
{

}
