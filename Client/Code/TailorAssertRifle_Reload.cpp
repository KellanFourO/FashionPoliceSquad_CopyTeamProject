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
    m_pHost = dynamic_cast<CPaintShotGun*>(Rifle);
    m_bAttack = false;
    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
    //Rifle->m_fGunMoveDown += m_fMoveDownMax;
}

CPlayerGunState* CTailorAssertRifle_Reload::Update(CPlayerGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        //Rifle->m_fGunMoveRight += m_fMoveRightSum;
        //Rifle->m_fGunMoveDown += m_fMoveDownSum;
    }
	//if (m_fBehaviorTime >= 0.1f) {
	//    if (m_fBehaviorTime >= 0.1f) {
	//        // 재장전 시
	//        if (Rifle->m_fMaxBullet > 0 && Rifle->m_fBulletCount < 50) {
	//            _int bulletsToReload = min(50 - Rifle->m_fBulletCount, Rifle->m_fMaxBullet);
	//            Rifle->m_fBulletCount += bulletsToReload;
	//            Rifle->m_fMaxBullet -= bulletsToReload;
	//            Rifle->m_bIsBullet = true;
	//        }
	//        // 총알이 다 떨어진 경우
	//        if (Rifle->m_fMaxBullet == 0 && Rifle->m_fBulletCount == 0) {
	//            Rifle->m_bIsBullet = false; // 총알이 다 떨어진 경우 발사 불가능
	//        }
	//    }
	//}
	//if (m_fBehaviorTime >= 0.2f) {
	//    Rifle->m_bReload = false;
	//    return new CRifle_IDLE;
	//}

    return nullptr;
}

void CTailorAssertRifle_Reload::Release(CPlayerGun* Rifle)
{
	m_pHost->Reset_GunMoveDown();
	m_pHost->Reset_GunMoveRight();
	//Rifle->m_fGunMoveRight = 3.f;
	//Rifle->m_fGunMoveDown = 1.f;
}
