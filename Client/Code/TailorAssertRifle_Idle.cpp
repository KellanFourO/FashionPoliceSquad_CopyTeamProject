#include "stdafx.h"
#include "TailorAssertRifle_Idle.h"
#include "Export_System.h"
#include "Export_Utility.h"



CTailorAssertRifle_Idle::CTailorAssertRifle_Idle()
{


}

CTailorAssertRifle_Idle::~CTailorAssertRifle_Idle()
{
}
void CTailorAssertRifle_Idle::Initialize(CPlayerGun* Rifle)
{
	m_pHost = Rifle;

	_vec3   vPlayerPos;
	m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);

	m_vPrePos = vPlayerPos;

	m_bAttack = false;
}

CPlayerGunState* CTailorAssertRifle_Idle::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
	m_fBehaviorTime += fTimeDelta;

	_vec3   vPlayerPos;
	m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);


	if (m_vPrePos != vPlayerPos) {
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(1); // Walk
	}
	m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김

	if (m_pHost->Get_Fire() && m_pHost->Get_GunInfo()->m_iCurrentBullet > 0)
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(4);

	else if (m_pHost->Get_GunInfo()->m_iCurrentBullet <= 0)
	{
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(5);
	}

    return nullptr;
}

void CTailorAssertRifle_Idle::Release(CPlayerGun* Rifle)
{
	m_fBehaviorTime = 0.f;
}
