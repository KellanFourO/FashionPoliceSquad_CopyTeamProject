#include "stdafx.h"
#include "Belt_Idle.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBelt_Idle::CBelt_Idle()
{


}

CBelt_Idle::~CBelt_Idle()
{
}

void CBelt_Idle::Initialize(CBelt* Belt)
{
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
    m_bAttack = false;
    m_pHost = Belt;
}

CBeltState* CBelt_Idle::Update(CBelt* Belt, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;
    //                  0       1       2           3       4       5
    //enum BELTSTATE { IDLE, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END };

    //todo Key_Pressing 눌리고 있는 경우
    //todo Key_Down 이전에 눌린적이 없고 현재 눌렀을 경우
    //todo Key_Up 이전에는 눌린 적이 있고 현재 눌리지 않았을 경우
    //DIKEYBOARD_LSHIFT

    if (Engine::Key_Down(DIK_LSHIFT))
    {
        m_fBehaviorTime = 0.f;
        return m_pHost->Get_State(1); // Attack
    }
    else
    {
        return nullptr;
    }


}

void CBelt_Idle::Release(CBelt* Belt)
{
    Belt->m_fBeltMoveRight = 4;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
    m_fBehaviorTime = 0;
}
