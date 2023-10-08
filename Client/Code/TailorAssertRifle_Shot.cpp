#include "stdafx.h"
#include "TailorAssertRifle_Shot.h"
#include "Export_System.h"
#include "Export_Utility.h"

CTailorAssertRifle_Shot::CTailorAssertRifle_Shot()
{


}

CTailorAssertRifle_Shot::~CTailorAssertRifle_Shot()
{
}
void CTailorAssertRifle_Shot::Initialize(CPlayerGun* Rifle)
{
    m_pHost = Rifle;

    m_vBaseScale = m_pHost->Get_Scale();

    m_fMoveRightSum = -0.07f;
    m_fMoveDownSum = 0.04f;
    m_fMoveRightMax = 0.3f;
    m_fMoveUpMax = 0.08f;

    m_fScaleMax = 1.1f;
    m_fScaleReduce = 0.99f;

    m_pHost->Mul_GunScale(m_fScaleMax);

    m_pHost->Add_GunMoveRight(m_fMoveRightMax);
    m_pHost->Reduce_GunMoveDown(m_fMoveUpMax);


}

CPlayerGunState* CTailorAssertRifle_Shot::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        m_pHost->Mul_GunScale(m_fScaleReduce);
    }

    if (m_bAttack)
    {
        m_pHost->Fire();
        dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_Flash()->Set_Fire(true);
        dynamic_cast<CTailorAssertRifle*>(m_pHost)->Set_Index(1);
        m_bAttack = false;
    }

    if (m_pHost->Get_GunMoveRight() <= 3.f)
    {
        m_pHost->Set_Fire(false);
        m_bAttack = true;
        return dynamic_cast<CTailorAssertRifle*>(m_pHost)->Get_State(0);
    }

    return nullptr;
}

void CTailorAssertRifle_Shot::Release(CPlayerGun* Rifle)
{
    m_fBehaviorTime = 0.f;
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
    m_pHost->Set_Scale(m_vBaseScale);
}
