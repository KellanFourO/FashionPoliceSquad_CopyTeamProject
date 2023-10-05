#include "stdafx.h"
#include "..\Header\Belt_CHARGE.h"
#include "BeltState.h"
#include "Belt_ATTACK.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Belt_CHARGE_Walk.h"
//#include "..\Header\Belt_CHARGE_Attack.h"

CBelt_CHARGE::CBelt_CHARGE()
{


}

CBelt_CHARGE::~CBelt_CHARGE()
{
}
void CBelt_CHARGE::Initialize(CBelt* Belt)
{
    Belt->m_bCharged = true;
}

CBeltState* CBelt_CHARGE::Update(CBelt* Belt, const float& fDeltaTime)
{

    if (!(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)) {
        return new CBelt_ATTACK;
    }

    return nullptr;
}

void CBelt_CHARGE::Release(CBelt* Belt)
{
    Belt->m_fBeltMoveRight = 4.f;
    Belt->m_fBeltMoveDown = 1.5f;
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
    Belt->m_bCharged = false;
}
