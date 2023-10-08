#include "stdafx.h"
#include "PaintShotGun_Idle.h"

#include "Export_System.h"
#include "Export_Utility.h"


CPaintShotGun_Idle::CPaintShotGun_Idle()
{


}

CPaintShotGun_Idle::~CPaintShotGun_Idle()
{
}
void CPaintShotGun_Idle::Initialize(CPlayerGun* ShotGun)
{
    m_pHost = ShotGun;

    _vec3   vPlayerPos;
    m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);

    m_vPrePos = vPlayerPos;

    m_bAttack = false;
}

CPlayerGunState* CPaintShotGun_Idle::Update(CPlayerGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    _vec3   vPlayerPos;
    m_pHost->Get_HostTransform()->Get_Info(INFO_POS,&vPlayerPos);

	if (m_vPrePos != vPlayerPos) {
	    return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(1); // Walk
	}
    m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김

    if(m_pHost->Get_Fire() && m_pHost->Get_Ready() && m_pHost->Get_GunInfo()->m_iCurrentBullet > 0)
        return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(4); // Shot

    else if (m_pHost->Get_RBFire())
    {
        return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(6); // Lazer
    }

    else if(m_pHost->Get_GunInfo()->m_iCurrentBullet <= 0)
    {
        return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(5);
    }


    return nullptr;
}

void CPaintShotGun_Idle::Release(CPlayerGun* ShotGun)
{
    m_fBehaviorTime = 0.f;
}
