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
    m_pHost = dynamic_cast<CPaintShotGun*>(Rifle);
    m_bAttack = false;
    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.1f;
    m_fMoveDownMax = 1.0f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
    //Rifle->m_fGunMoveDown += m_fMoveDownMax;
}

CPlayerGunState* CTailorAssertRifle_Ready::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        //Rifle->m_fGunMoveRight += m_fMoveRightSum;
        //Rifle->m_fGunMoveDown += m_fMoveDownSum;
    }
	//if (m_fBehaviorTime >= 0.2f) {
	//    Rifle->m_bReady = false;
	//    return new CRifle_IDLE;
	//}
    //__super::Update(Rifle, fTimeDelta);
    return nullptr;
}

void CTailorAssertRifle_Ready::Release(CPlayerGun* Rifle)
{
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
    //Rifle->m_fGunMoveRight = 3.f;
    //Rifle->m_fGunMoveDown = 1.f;


}
