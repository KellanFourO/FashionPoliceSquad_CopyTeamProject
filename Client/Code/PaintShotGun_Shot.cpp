#include "stdafx.h"
#include "PaintShotGun_Shot.h"
#include "MuzzleFlash.h"

#include "Export_System.h"
#include "Export_Utility.h"


CPaintShotGun_Shot::CPaintShotGun_Shot()
{


}

CPaintShotGun_Shot::~CPaintShotGun_Shot()
{
}
void CPaintShotGun_Shot::Initialize(CPlayerGun* ShotGun)
{
    m_pHost = dynamic_cast<CPaintShotGun*>(ShotGun);


    m_vBaseScale = m_pHost->Get_Scale();
    //m_vBaseScale = ShotGun->m_vGunScale;

    m_fMoveRightSum = -0.03f;
    m_fMoveDownSum = 0.005f;
    m_fMoveRightMax = 0.9f;
    m_fMoveUpMax = 0.2f;

    m_fScaleMax = 1.45f;
    m_fScaleReduce = 0.99f;

    m_pHost->Mul_GunScale(m_fScaleMax);
    //ShotGun->m_vGunScale *= m_fScaleMax;

    m_pHost->Add_GunMoveRight(m_fMoveRightMax);
    m_pHost->Reduce_GunMoveDown(m_fMoveUpMax);

    //ShotGun->m_fGunMoveRight += m_fMoveRightMax;
    //ShotGun->m_fGunMoveDown -= m_fMoveUpMax;

    SoundMgr()->PlaySoundW(L"ShotGun_Fire.wav", SOUND_GUN,10);
}

CPlayerGunState* CPaintShotGun_Shot::Update(CPlayerGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        m_pHost->Mul_GunScale(m_fScaleReduce);
        //ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        //ShotGun->m_fGunMoveDown += m_fMoveDownSum;
        //ShotGun->m_vGunScale*= m_fScaleReduce;
    }

    if (m_bAttack)
    {
		m_pHost->Fire();
        
        dynamic_cast<CPaintShotGun*>(m_pHost)->Get_Flash()->Set_Fire(true);
        dynamic_cast<CPaintShotGun*>(m_pHost)->Set_Index(1);

        m_bAttack = false;

    }

    if (m_pHost->Get_GunMoveRight() <= 3.f)
    {
        m_pHost->Set_Fire(false);
        m_pHost->Set_Ready(false);
        m_bAttack = true;
        return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(3); // Ready
    }



    return nullptr;
}

void CPaintShotGun_Shot::Release(CPlayerGun* ShotGun)
{
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
    m_pHost->Set_Scale(m_vBaseScale);
    m_fBehaviorTime = 0.f;
    //ShotGun->m_fGunMoveRight = 3.f;
    //ShotGun->m_fGunMoveDown = 1.f;
    SoundMgr()->StopSound(SOUND_GUN);
}
