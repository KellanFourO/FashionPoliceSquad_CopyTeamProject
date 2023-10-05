#include "stdafx.h"
#include "..\Header\ShotGun_RELOAD.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_RELOAD_RELOAD.h"
//#include "..\Header\ShotGun_RELOAD_Attack.h"

CShotGun_RELOAD::CShotGun_RELOAD()
{


}

CShotGun_RELOAD::~CShotGun_RELOAD()
{
}
void CShotGun_RELOAD::Initialize(CGun* ShotGun)
{
    m_pHost = dynamic_cast<CDyehard*>(ShotGun);

    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    ShotGun->m_fGunMoveDown += m_fMoveDownMax;
}

CShotGunState* CShotGun_RELOAD::Update(CGun* ShotGun, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime >= 0.02f) {
        ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        ShotGun->m_fGunMoveDown += m_fMoveDownSum;
    }
    if (m_fBehaviorTime >= 0.1f) {
        // 재장전 시
        if (ShotGun->m_fMaxBullet > 0 && ShotGun->m_fBulletCount < 6) {
            _int bulletsToReload = min(6 - ShotGun->m_fBulletCount, ShotGun->m_fMaxBullet);
            ShotGun->m_fBulletCount += bulletsToReload;
            ShotGun->m_fMaxBullet -= bulletsToReload;
            ShotGun->m_bIsBullet = true;
        }
        // 총알이 다 떨어진 경우
        if (ShotGun->m_fMaxBullet == 0 && ShotGun->m_fBulletCount == 0) {
            ShotGun->m_bIsBullet = false; // 총알이 다 떨어진 경우 발사 불가능
        }
    }

    if (m_fBehaviorTime >= 0.2f) {
        ShotGun->m_bReload = false;
        return m_pHost->Get_State(0); // Idle
    }
    if (ShotGun->m_bReady) {
        return m_pHost->Get_State(3); // Ready
    }
    return nullptr;
}

void CShotGun_RELOAD::Release(CGun* ShotGun)
{
    ShotGun->m_fGunMoveRight = 3.f;
    ShotGun->m_fGunMoveDown = 1.f;
}
