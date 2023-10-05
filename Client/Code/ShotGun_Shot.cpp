#include "stdafx.h"
#include "..\Header\ShotGun_SHOT.h"

#include "ShotGun_IDLE.h"
#include "ShotGun_READY.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_SHOT_SHOT.h"
//#include "..\Header\ShotGun_SHOT_Attack.h"

CShotGun_SHOT::CShotGun_SHOT()
{


}

CShotGun_SHOT::~CShotGun_SHOT()
{
}
void CShotGun_SHOT::Initialize(CGun* ShotGun)
{
    m_pHost = dynamic_cast<CDyehard*>(ShotGun);
    m_bAttack = false;

    m_vBaseScale = ShotGun->m_vGunScale;

    m_fMoveRightSum = -0.03f;
    m_fMoveDownSum = 0.005f;
    m_fMoveRightMax = 0.9f;
    m_fMoveUpMax = 0.2f;

    m_fScaleMax = 1.45f;
    m_fScaleReduce = 0.99f;

    ShotGun->m_vGunScale *= m_fScaleMax;

    ShotGun->m_fGunMoveRight += m_fMoveRightMax;
    ShotGun->m_fGunMoveDown -= m_fMoveUpMax;


}

CShotGunState* CShotGun_SHOT::Update(CGun* ShotGun, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime >= 0.02f) {
        ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        ShotGun->m_fGunMoveDown += m_fMoveDownSum;
        ShotGun->m_vGunScale*= m_fScaleReduce;
    }

    if (ShotGun->m_fGunMoveRight <= 3.f)
    {
        return m_pHost->Get_State(0); // Idle
    }
    if (ShotGun->m_bReady) {
        return m_pHost->Get_State(3); // Ready
    }
    return nullptr;
}

void CShotGun_SHOT::Release(CGun* ShotGun)
{
    ShotGun->m_fGunMoveRight = 3.f;
    ShotGun->m_fGunMoveDown = 1.f;
    ShotGun->m_vGunScale = m_vBaseScale;
}
