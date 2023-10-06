#include "stdafx.h"
#include "..\Header\Belt_IDLE.h"
#include "BeltState.h"
#include "Belt_NORMAL.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Belt_IDLE_Walk.h"
//#include "..\Header\Belt_IDLE_Attack.h"

CBelt_IDLE::CBelt_IDLE()
{


}

CBelt_IDLE::~CBelt_IDLE()
{
}
void CBelt_IDLE::Initialize(CBelt* Belt)
{
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
    m_bAttack = false;
}

CBeltState* CBelt_IDLE::Update(CBelt* Belt, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80) {
        return new CBelt_NORMAL;
    }


    return nullptr;
}

void CBelt_IDLE::Release(CBelt* Belt)
{
    Belt->m_fBeltMoveRight = 4;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
}
