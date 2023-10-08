#include "stdafx.h"
#include "TailorAssertRifle_Walk.h"
#include "Export_System.h"
#include "Export_Utility.h"


CTailorAssertRifle_Walk::CTailorAssertRifle_Walk()
{


}

CTailorAssertRifle_Walk::~CTailorAssertRifle_Walk()
{
}
void CTailorAssertRifle_Walk::Initialize(CPlayerGun* Rifle)
{
    m_pHost = Rifle;

	_vec3   vPlayerPos;
	m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);
    m_vPrePos = vPlayerPos;

    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.01f;

}

CPlayerGunState* CTailorAssertRifle_Walk::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);

        if (m_pHost->Get_GunMoveRight() >= 3.2f || m_pHost->Get_GunMoveRight() < 2.8f) {
            m_fMoveRightSum = -m_fMoveRightSum;
            m_fMoveDownSum = -m_fMoveDownSum;// 방향 반전
        }
        if (m_pHost->Get_GunMoveRight() < 3.009f && m_pHost->Get_GunMoveRight() > 2.991f) {
            m_pHost->Reset_GunMoveDown();
            m_pHost->Reset_GunMoveRight();

            m_fMoveDownSum = -m_fMoveDownSum;
        }

        m_fBehaviorTime = 0.f;
    }


	_vec3   vPlayerPos;
	m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);

    if (m_vPrePos == vPlayerPos)
    {
        return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(0); // IDLE
    }

	m_vPrePos = vPlayerPos;

	if (m_pHost->Get_Fire() && m_pHost->Get_GunInfo()->m_iCurrentBullet > 0)
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(4); // SHOT

	else if (m_pHost->Get_GunInfo()->m_iCurrentBullet <= 0)
	{
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(5); // RELOAD
	}

    return nullptr;
}

void CTailorAssertRifle_Walk::Release(CPlayerGun* Rifle)
{
    m_pHost->Reset_GunMoveRight();
    m_pHost->Reset_GunMoveDown();

}
