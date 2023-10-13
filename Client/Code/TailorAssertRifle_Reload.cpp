#include "stdafx.h"
#include "TailorAssertRifle_Reload.h"
#include "Export_System.h"
#include "Export_Utility.h"

CTailorAssertRifle_Reload::CTailorAssertRifle_Reload()
{


}

CTailorAssertRifle_Reload::~CTailorAssertRifle_Reload()
{
}
void CTailorAssertRifle_Reload::Initialize(CPlayerGun* Rifle)
{
    m_pHost = Rifle;
    m_bAttack = false;
    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
}

CPlayerGunState* CTailorAssertRifle_Reload::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
    }

	if (m_fBehaviorTime >= 0.1f && m_pHost->Get_GunInfo()->m_iMaxBullet != 0)
	{
		m_pHost->Reload(0,0);
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(2); //Ready
	}

	else if (m_pHost->Get_GunInfo()->m_iMaxBullet == 0)
	{
		return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(0); //Idle
	}


    return nullptr;
}

void CTailorAssertRifle_Reload::Release(CPlayerGun* Rifle)
{
    m_fBehaviorTime = 0.f;
	m_pHost->Reset_GunMoveDown();
	m_pHost->Reset_GunMoveRight();
}
