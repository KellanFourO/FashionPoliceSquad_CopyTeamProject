#include "stdafx.h"
#include "..\Header\ShotGun_READY.h"
#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_READY_READY.h"
//#include "..\Header\ShotGun_READY_Attack.h"

CShotGun_READY::CShotGun_READY()
{


}

CShotGun_READY::~CShotGun_READY()
{
}
void CShotGun_READY::Initialize(CGun* ShotGun)
{
    m_pHost = dynamic_cast<CDyehard*>(ShotGun);

    m_bAttack = false;
    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.1f;
    m_fMoveDownMax = 1.0f;
    ShotGun->m_fGunMoveDown += m_fMoveDownMax;
}

CShotGunState* CShotGun_READY::Update(CGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        ShotGun->m_fGunMoveDown += m_fMoveDownSum;
    }
    if (m_fBehaviorTime >= 0.2f) {
        ShotGun->m_bReady = false;
        return m_pHost->Get_State(0); // Idle
    }

    return nullptr;
}

void CShotGun_READY::Release(CGun* ShotGun)
{
    ShotGun->m_fGunMoveRight = 3.f;
    ShotGun->m_fGunMoveDown = 1.f;
}
