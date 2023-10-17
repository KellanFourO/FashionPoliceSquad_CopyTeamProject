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

    //                  0       1       2           3       4       5
    //enum BELTSTATE { IDLE, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END };

    //todo Key_Pressing ������ �ִ� ���
    //todo Key_Down ������ �������� ���� ���� ������ ���
    //todo Key_Up �������� ���� ���� �ְ� ���� ������ �ʾ��� ���


    if (Key_Down(DIK_LSHIFT))
    {
        return m_pHost->Get_State(1); // Ready
    }

    return nullptr;

}

void CBelt_Idle::Release(CBelt* Belt)
{
    Belt->m_fBeltMoveRight = 4;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);

}
