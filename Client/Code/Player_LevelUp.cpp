#include "stdafx.h"
#include "..\Header\Player.h"

#include "Player_LevelUp.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "EventMgr.h"


CPlayer_LevelUp::CPlayer_LevelUp()
{
}

CPlayer_LevelUp::~CPlayer_LevelUp()
{
}

void CPlayer_LevelUp::Initialize(CPlayer* Player)
{
    m_pHost = Player;
    StateID = PlayerStateID::Player_Lv_Up;
    CEventMgr::GetInstance()->OnLevelUp(m_pHost->Get_GraphicDev(), SCENETAG::STAGE);
}

CPlayerState* CPlayer_LevelUp::Update(CPlayer* Player, const float& fTimeDelta)
{
     m_fBehaviorTime += fTimeDelta;


     if (m_fBehaviorTime > 0.6f) {
         return m_pHost->Get_State(0);
     }

    return nullptr;
}

void CPlayer_LevelUp::Release(CPlayer* Player)
{
    m_fBehaviorTime = 0.f;
}
