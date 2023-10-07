#include "stdafx.h"
#include "PaintShotGun_Enter.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPaintShotGun_Enter::CPaintShotGun_Enter()
{


}

CPaintShotGun_Enter::~CPaintShotGun_Enter()
{
}
void CPaintShotGun_Enter::Initialize(CPlayerGun* ShotGun)
{

    m_pHost = dynamic_cast<CPaintShotGun*>(ShotGun);

    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
    //ShotGun->m_fGunMoveDown += m_fMoveDownMax;
}

CPlayerGunState* CPaintShotGun_Enter::Update(CPlayerGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
		//fShotGun->m_fGunMoveRight += m_fMoveRightSum;
		//fShotGun->m_fGunMoveDown += m_fMoveDownSum;
    }
	//if (m_fBehaviorTime >= 0.2f) {
	//    //ShotGun->m_bENTER = false;
	//    return m_pHost->Get_State(0); // Idle
	//}
	//if (ShotGun->m_bReady) {
	//    return m_pHost->Get_State(3); // Ready
	//}

    return nullptr;
}

void CPaintShotGun_Enter::Release(CPlayerGun* ShotGun)
{
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
    //ShotGun->m_fGunMoveRight = 3.f;
    //ShotGun->m_fGunMoveDown = 1.f;
    //ShotGun->m_bIsBullet = true;
    //ShotGun->m_fBulletCount = 6.f;
    //ShotGun->m_fMaxBullet -= ShotGun->m_fBulletCount;
}
