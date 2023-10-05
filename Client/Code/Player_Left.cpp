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
}

CPlayerState* CPlayer_Left::Update(CPlayer* Player, const float& fDeltaTime)
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

void CPlayer_Left::Release(CPlayer* Player)
{

}
