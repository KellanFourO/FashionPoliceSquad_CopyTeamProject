#include "stdafx.h"
#include "PaintShotGun_Lazer.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPaintShotGun_Lazer::CPaintShotGun_Lazer()
{


}

CPaintShotGun_Lazer::~CPaintShotGun_Lazer()
{
}
void CPaintShotGun_Lazer::Initialize(CPlayerGun* ShotGun)
{
    m_bAttack = false;
    m_pHost = dynamic_cast<CPaintShotGun*>(ShotGun);
    m_fMoveDownMax = -0.2f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);
    //ShotGun->m_fGunMoveDown += m_fMoveDownMax;
}

CPlayerGunState* CPaintShotGun_Lazer::Update(CPlayerGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;



    //if (!Engine::Get_DIMouseState(DIM_RB)) {
    //    m_pHost->m_bLazer = false;
    //    return m_pHost->Get_State(0);
    //}

    return nullptr;
}

void CPaintShotGun_Lazer::Release(CPlayerGun* ShotGun)
{
    m_pHost->Reset_GunMoveDown();
    m_pHost->Reset_GunMoveRight();
	//ShotGun->m_fGunMoveRight = 3.f;
	//ShotGun->m_fGunMoveDown = 1.f;
    //dynamic_cast<CPaintShotGun*>(ShotGun)->m_bLazer = false;
}