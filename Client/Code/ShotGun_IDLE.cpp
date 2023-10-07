#include "stdafx.h"
#include "..\Header\ShotGun_IDLE.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_IDLE_Walk.h"
//#include "..\Header\ShotGun_IDLE_Attack.h"

CShotGun_IDLE::CShotGun_IDLE()
{


}

CShotGun_IDLE::~CShotGun_IDLE()
{
}
void CShotGun_IDLE::Initialize(CGun* ShotGun)
{
    m_pHost = dynamic_cast<CDyehard*>(ShotGun);

    _vec3   vPlayerPos;
    m_pHost->m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

    m_vPrePos = vPlayerPos;

    m_bAttack = false;
}

CShotGunState* CShotGun_IDLE::Update(CGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    _vec3   vPlayerPos;
    m_pHost->m_pPlayerTransformCom->Get_Info(INFO_POS,&vPlayerPos);

    if (m_vPrePos != vPlayerPos) {
        return m_pHost->Get_State(1); // Walk
    }
    m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김

    if (ShotGun->m_bReload == true) {
        return m_pHost->Get_State(5);
    }

    if (ShotGun->m_bReady) {
        return m_pHost->Get_State(3);
    }

    dynamic_cast<CDyehard*>(ShotGun)->Gun_Fire();


    return nullptr;
}

void CShotGun_IDLE::Release(CGun* ShotGun)
{
}
