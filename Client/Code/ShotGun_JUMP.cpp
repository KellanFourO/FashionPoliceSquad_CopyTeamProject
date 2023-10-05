#include "stdafx.h"
#include "..\Header\ShotGun_JUMP.h"
#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_JUMP_JUMP.h"
//#include "..\Header\ShotGun_JUMP_Attack.h"

CShotGun_JUMP::CShotGun_JUMP()
{


}

CShotGun_JUMP::~CShotGun_JUMP()
{
}
void CShotGun_JUMP::Initialize(CGun* ShotGun)
{
    m_pHost = dynamic_cast<CDyehard*>(ShotGun);
    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.01f;

}

CShotGunState* CShotGun_JUMP::Update(CGun* ShotGun, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime >= 0.02f) {
        ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        ShotGun->m_fGunMoveDown += m_fMoveDownSum;
        if (ShotGun->m_fGunMoveRight >= 3.2f || ShotGun->m_fGunMoveRight < 2.8f) {
            m_fMoveRightSum = -m_fMoveRightSum;
            m_fMoveDownSum = -m_fMoveDownSum;// 방향 반전
        }
        if (ShotGun->m_fGunMoveRight < 3.017f && ShotGun->m_fGunMoveRight > 2.983f) {
            ShotGun->m_fGunMoveRight = 3.f;
            ShotGun->m_fGunMoveDown = 1.f;
            m_fMoveDownSum = -m_fMoveDownSum;
        }

        m_fBehaviorTime = 0.f;
    }

    _vec3   vPlayerPos;
    m_pHost->m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

    if (m_vPrePos == vPlayerPos) {
        return m_pHost->Get_State(0); // Idle
    }
    m_vPrePos = vPlayerPos;
    if (ShotGun->m_bReload == true) {
        return m_pHost->Get_State(5); // Reload
    }
    if (ShotGun->m_bReady) {
        return m_pHost->Get_State(3); // Ready
    }

    return nullptr;
}

void CShotGun_JUMP::Release(CGun* ShotGun)
{
    ShotGun->m_fGunMoveRight = 3.f;
    ShotGun->m_fGunMoveDown = 1.f;
}
