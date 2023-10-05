#include "stdafx.h"
#include "..\Header\ShotGun_Lazer.h"

#include "ShotGun_IDLE.h"
#include "ShotGun_READY.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_Lazer_Lazer.h"
//#include "..\Header\ShotGun_Lazer_Attack.h"

CShotGun_Lazer::CShotGun_Lazer()
{


}

CShotGun_Lazer::~CShotGun_Lazer()
{
}
void CShotGun_Lazer::Initialize(CGun* ShotGun)
{
    m_bAttack = false;
    m_pHost = dynamic_cast<CDyehard*>(ShotGun);
    m_fMoveDownMax = -0.2f;
    ShotGun->m_fGunMoveDown += m_fMoveDownMax;
}

CShotGunState* CShotGun_Lazer::Update(CGun* ShotGun, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;



    if (!Engine::Get_DIMouseState(DIM_RB)) {
        m_pHost->m_bLazer = false;
        return m_pHost->Get_State(0);
    }

    return nullptr;
}

void CShotGun_Lazer::Release(CGun* ShotGun)
{

    ShotGun->m_fGunMoveRight = 3.f;
    ShotGun->m_fGunMoveDown = 1.f;
    dynamic_cast<CDyehard*>(ShotGun)->m_bLazer = false;
}
