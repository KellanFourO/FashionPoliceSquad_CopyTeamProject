#include "stdafx.h"
#include "..\Header\Rifle_RELOAD.h"

#include "Rifle_IDLE.h"
#include "Rifle_READY.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Rifle_RELOAD_RELOAD.h"
//#include "..\Header\Rifle_RELOAD_Attack.h"

CRifle_RELOAD::CRifle_RELOAD()
{


}

CRifle_RELOAD::~CRifle_RELOAD()
{
}
void CRifle_RELOAD::Initialize(CGun* Rifle)
{
    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    Rifle->m_fGunMoveDown += m_fMoveDownMax;
}

CRifleState* CRifle_RELOAD::Update(CGun* Rifle, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime >= 0.02f) {
        Rifle->m_fGunMoveRight += m_fMoveRightSum;
        Rifle->m_fGunMoveDown += m_fMoveDownSum;
    }
    if (m_fBehaviorTime >= 0.1f) {
        if (m_fBehaviorTime >= 0.1f) {
            // 재장전 시
            if (Rifle->m_fMaxBullet > 0 && Rifle->m_fBulletCount < 50) {
                _int bulletsToReload = min(50 - Rifle->m_fBulletCount, Rifle->m_fMaxBullet);
                Rifle->m_fBulletCount += bulletsToReload;
                Rifle->m_fMaxBullet -= bulletsToReload;
                Rifle->m_bIsBullet = true;
            }
            // 총알이 다 떨어진 경우
            if (Rifle->m_fMaxBullet == 0 && Rifle->m_fBulletCount == 0) {
                Rifle->m_bIsBullet = false; // 총알이 다 떨어진 경우 발사 불가능
            }
        }
    }
    if (m_fBehaviorTime >= 0.2f) {
        Rifle->m_bReload = false;
        return new CRifle_IDLE;
    }

    return nullptr;
}

void CRifle_RELOAD::Release(CGun* Rifle)
{
    Rifle->m_fGunMoveRight = 3.f;
    Rifle->m_fGunMoveDown = 1.f;
}
