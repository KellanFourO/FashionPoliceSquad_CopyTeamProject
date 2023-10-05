#include "stdafx.h"
#include "..\Header\Rifle_READY.h"

#include "Rifle_IDLE.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Rifle_READY_READY.h"
//#include "..\Header\Rifle_READY_Attack.h"

CRifle_READY::CRifle_READY()
{


}

CRifle_READY::~CRifle_READY()
{
}
void CRifle_READY::Initialize(CGun* Rifle)
{
    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.1f;
    m_fMoveDownMax = 1.0f;
    Rifle->m_fGunMoveDown += m_fMoveDownMax;
}

CRifleState* CRifle_READY::Update(CGun* Rifle, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime >= 0.02f) {
        Rifle->m_fGunMoveRight += m_fMoveRightSum;
        Rifle->m_fGunMoveDown += m_fMoveDownSum;
    }
    if (m_fBehaviorTime >= 0.2f) {
        Rifle->m_bReady = false;
        return new CRifle_IDLE;
    }
    //__super::Update(Rifle, fDeltaTime);
    return nullptr;
}

void CRifle_READY::Release(CGun* Rifle)
{
    Rifle->m_fGunMoveRight = 3.f;
    Rifle->m_fGunMoveDown = 1.f;


}
