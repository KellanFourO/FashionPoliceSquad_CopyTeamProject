#include "stdafx.h"
#include "Belt_Charge.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Belt_CHARGE_Walk.h"
//#include "..\Header\Belt_CHARGE_Attack.h"

CBelt_Charge::CBelt_Charge()
{


}

CBelt_Charge::~CBelt_Charge()
{
}
void CBelt_Charge::Initialize(CBelt* Belt)
{
    Belt->m_bCharged = true;
    m_pHost = Belt;
}

CBeltState* CBelt_Charge::Update(CBelt* Belt, const float& fTimeDelta)
{

    if (!(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)) {
        return m_pHost->Get_State(2); // CHARGEATTACK
    }

    return nullptr;
}

void CBelt_Charge::Release(CBelt* Belt)
{
    Belt->m_fBeltMoveRight = 4.f;
    Belt->m_fBeltMoveDown = 1.5f;
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
    Belt->m_bCharged = false;
}
