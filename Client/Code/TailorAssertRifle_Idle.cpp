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
	m_pHost = dynamic_cast<CPaintShotGun*>(Rifle);


    _vec3   vPlayerPos;
	m_pHost->Get_Transform()->Get_Info(INFO_POS, &vPlayerPos);
    m_vPrePos = vPlayerPos;

    m_bAttack = false;
}

CPlayerGunState* CTailorAssertRifle_Idle::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;
	_vec3   vPlayerPos;
	m_pHost->Get_Transform()->Get_Info(INFO_POS, &vPlayerPos);

	//if (m_vPrePos != vPlayerPos) {
	//    return new CRifle_WALK;
	//}
	//m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김
	//
	//if (Rifle->m_bReload == true) {
	//    return new CRifle_RELOAD;
	//}
	//
	//dynamic_cast<CRifle*>(Rifle)->Gun_Fire();
	//
	//if (Rifle->m_bReady) {
	//    return new CRifle_READY;
	//}

    return nullptr;
}

void CTailorAssertRifle_Idle::Release(CPlayerGun* Rifle)
{
}
