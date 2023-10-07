#include "stdafx.h"
#include "TailorAssertRifle_Ready.h"

#include "Export_System.h"
#include "Export_Utility.h"

CTailorAssertRifle_Ready::CTailorAssertRifle_Ready()
{


}

CTailorAssertRifle_Ready::~CTailorAssertRifle_Ready()
{
}
void CTailorAssertRifle_Ready::Initialize(CPlayerGun* Rifle)
{
    m_pHost = Rifle;
    m_bAttack = false;
    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.1f;
    m_fMoveDownMax = 1.0f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
}

CPlayerGunState* CTailorAssertRifle_Ready::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
    }
	if (m_fBehaviorTime >= 0.2f) {
        return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(0);
	}
    return nullptr;
}

void CTailorAssertRifle_Ready::Release(CPlayerGun* Rifle)
{
    m_fBehaviorTime = 0.f;
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
}
