#include "stdafx.h"
#include "TailorAssertRifle_Shot.h"

#include "Rifle_IDLE.h"
#include "Rifle_READY.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Rifle_SHOT_SHOT.h"
//#include "..\Header\Rifle_SHOT_Attack.h"

CTailorAssertRifle_Shot::CTailorAssertRifle_Shot()
{


}

CTailorAssertRifle_Shot::~CTailorAssertRifle_Shot()
{
}
void CTailorAssertRifle_Shot::Initialize(CPlayerGun* Rifle)
{
    m_bAttack = false;

    m_vBaseScale = Rifle->m_vGunScale;

    m_fMoveRightSum = -0.07f;
    m_fMoveDownSum = 0.04f;
    m_fMoveRightMax = 0.3f;
    m_fMoveUpMax = 0.08f;

    m_fScaleMax = 1.1f;
    m_fScaleReduce = 0.99f;

    Rifle->m_vGunScale *= m_fScaleMax;

    Rifle->m_fGunMoveRight += m_fMoveRightMax;
    Rifle->m_fGunMoveDown -= m_fMoveUpMax;


}

CRifleState* CTailorAssertRifle_Shot::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        Rifle->m_fGunMoveRight += m_fMoveRightSum;
        Rifle->m_fGunMoveDown += m_fMoveDownSum;
        Rifle->m_vGunScale *= m_fScaleReduce;
    }

    if (Rifle->m_fGunMoveRight <= 3.f)
    {
        return new CRifle_IDLE;
    }
    if (Rifle->m_bReady) {
        return new CRifle_READY;
    }
    return nullptr;
}

void CTailorAssertRifle_Shot::Release(CPlayerGun* Rifle)
{
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
    m_pHost->Set_Scale(m_vBaseScale);
    //Rifle->m_fGunMoveRight = 3.f;
    //Rifle->m_fGunMoveDown = 1.f;
    //Rifle->m_vGunScale = m_vBaseScale;
}
