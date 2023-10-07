#include "stdafx.h"
#include "..\Header\ShotGun_ENTER.h"
#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_ENTER_ENTER.h"
//#include "..\Header\ShotGun_ENTER_Attack.h"

CShotGun_ENTER::CShotGun_ENTER()
{


}

CShotGun_ENTER::~CShotGun_ENTER()
{
}
void CShotGun_ENTER::Initialize(CGun* ShotGun)
{

    m_pHost = dynamic_cast<CDyehard*>(ShotGun);

    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    ShotGun->m_fGunMoveDown += m_fMoveDownMax;
}

CShotGunState* CShotGun_ENTER::Update(CGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        ShotGun->m_fGunMoveDown += m_fMoveDownSum;
    }
    if (m_fBehaviorTime >= 0.2f) {
        //ShotGun->m_bENTER = false;
        return m_pHost->Get_State(0); // Idle
    }
    if (ShotGun->m_bReady) {
        return m_pHost->Get_State(3); // Ready
    }

    return nullptr;
}

void CShotGun_ENTER::Release(CGun* ShotGun)
{
    ShotGun->m_fGunMoveRight = 3.f;
    ShotGun->m_fGunMoveDown = 1.f;
    ShotGun->m_bIsBullet = true;
    ShotGun->m_fBulletCount = 6.f;
    ShotGun->m_fMaxBullet -= ShotGun->m_fBulletCount;
}
