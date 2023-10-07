#include "stdafx.h"
#include "PaintShotGun_Jump.h"
#include "Export_System.h"
#include "Export_Utility.h"


CPaintShotGun_Jump::CPaintShotGun_Jump()
{


}

CPaintShotGun_Jump::~CPaintShotGun_Jump()
{
}
void CPaintShotGun_Jump::Initialize(CPlayerGun* ShotGun)
{
    m_pHost = dynamic_cast<CPaintShotGun*>(ShotGun);
    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.01f;

}

CPlayerGunState* CPaintShotGun_Jump::Update(CPlayerGun* ShotGun, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        //ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        //ShotGun->m_fGunMoveDown += m_fMoveDownSum;
        if (m_pHost->Get_GunMoveRight() >= 3.2f || m_pHost->Get_GunMoveRight() < 2.8f) {
            m_fMoveRightSum = -m_fMoveRightSum;
            m_fMoveDownSum = -m_fMoveDownSum;// 방향 반전
        }
        if (m_pHost->Get_GunMoveRight() < 3.017f && m_pHost->Get_GunMoveRight() > 2.983f) {
            m_pHost->Reset_GunMoveDown();
            m_pHost->Reset_GunMoveRight();
			//ShotGun->m_fGunMoveRight = 3.f;
			//ShotGun->m_fGunMoveDown = 1.f;
            m_fMoveDownSum = -m_fMoveDownSum;
        }

        m_fBehaviorTime = 0.f;
    }

    _vec3   vPlayerPos;
    m_pHost->Get_Transform()->Get_Info(INFO_POS, &vPlayerPos);

   // if (m_vPrePos == vPlayerPos) {
   //     return m_pHost->Get_State(0); // Idle
   // }
   // m_vPrePos = vPlayerPos;
   // if (ShotGun->m_bReload == true) {
   //     return m_pHost->Get_State(5); // Reload
   // }
   // if (ShotGun->m_bReady) {
   //     return m_pHost->Get_State(3); // Ready
   // }

    return nullptr;
}

void CPaintShotGun_Jump::Release(CPlayerGun* ShotGun)
{
    m_pHost->Reset_GunMoveRight();
    m_pHost->Reset_GunMoveDown();
    //ShotGun->m_fGunMoveRight = 3.f;
    //ShotGun->m_fGunMoveDown = 1.f;
}
