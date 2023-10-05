#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_Left.h"
#include "Player_Right.h"
#include "Player_Left.h"
#include "Player_Hit.h"
#include "Player_Lv_Up.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Player_Walk.h"
//#include "..\Header\Player_Attack.h"

CPlayer_Left::CPlayer_Left()
{
}

CPlayer_Left::~CPlayer_Left()
{
}

void CPlayer_Left::Initialize(CPlayer* Player)
{
    StateID = PlayerStateID::Player_Left;
}

CPlayerState* CPlayer_Left::Update(CPlayer* Player, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    Player->Key_Input(fDeltaTime);

    if (m_fBehaviorTime > 0.6f) {
        return new CPlayer_IDLE;
    }

    return nullptr;
}

void CPlayer_Left::Release(CPlayer* Player)
{

}
