#include "stdafx.h"
#include "PaintShotGun_Ready.h"
#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_READY_READY.h"
//#include "..\Header\ShotGun_READY_Attack.h"

CPaintShotGun_Ready::CPaintShotGun_Ready()
{


}

CPaintShotGun_Ready::~CPaintShotGun_Ready()
{
}
void CPaintShotGun_Ready::Initialize(CPlayerGun* ShotGun)
{
    m_pHost = dynamic_cast<CPaintShotGun*>(ShotGun);

    m_bAttack = false;
    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.1f;
    m_fMoveDownMax = 1.0f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
}

CPlayerGunState* CPaintShotGun_Ready::Update(CPlayerGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        m_pHost->Add_GunMoveRight(m_fMoveRightSum);
        m_pHost->Add_GunMoveDown(m_fMoveDownSum);
		//ShotGun->m_fGunMoveRight += m_fMoveRightSum;
		//ShotGun->m_fGunMoveDown += m_fMoveDownSum;
    }
	//if (m_fBehaviorTime >= 0.2f) {
	//    ShotGun->m_bReady = false;
	//    return m_pHost->Get_State(0); // Idle
	//}

    return nullptr;
}

void CPaintShotGun_Ready::Release(CPlayerGun* ShotGun)
{
	m_pHost->Reset_GunMoveDown();
	m_pHost->Reset_GunMoveRight();
    //ShotGun->m_fGunMoveRight = 3.f;
    //ShotGun->m_fGunMoveDown = 1.f;
}
