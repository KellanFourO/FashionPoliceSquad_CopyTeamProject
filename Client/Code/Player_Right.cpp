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
    m_pHost =   Player;
}

CPlayerState* CPlayer_Right::Update(CPlayer* Player, const float& fDeltaTime)
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

void CPlayer_Right::Release(CPlayer* Player)
{

}
