#include "stdafx.h"
#include "PaintShotGun_Reload.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\ShotGun_RELOAD_RELOAD.h"
//#include "..\Header\ShotGun_RELOAD_Attack.h"

CPaintShotGun_Reload::CPaintShotGun_Reload()
{


}

CPaintShotGun_Reload::~CPaintShotGun_Reload()
{
}
void CPaintShotGun_Reload::Initialize(CPlayerGun* ShotGun)
{
    m_pHost = ShotGun;

    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.06f;
    m_fMoveDownMax = 0.5f;
    m_pHost->Add_GunMoveDown(m_fMoveDownMax);

    SoundMgr()->PlaySoundW(L"ShotGun_Reload.wav",SOUND_PLAYER2, 1.f);
}

CPlayerGunState* CPaintShotGun_Reload::Update(CPlayerGun* ShotGun, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
		m_pHost->Add_GunMoveRight(m_fMoveRightSum);
		m_pHost->Add_GunMoveDown(m_fMoveDownSum);
        //ShotGun->m_fGunMoveRight += m_fMoveRightSum;
        //ShotGun->m_fGunMoveDown += m_fMoveDownSum;
    }

    if (m_fBehaviorTime >= 0.1f && m_pHost->Get_GunInfo()->m_iMaxBullet != 0)
    {
        m_pHost->Reload(dynamic_cast<CPaintShotGun*>(m_pHost)->Get_BulletColr(),0);
        dynamic_cast<CPaintShotGun*>(m_pHost)->Add_BulletColor();
        return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(3);
    }

    else if(m_pHost->Get_GunInfo()->m_iMaxBullet == 0)
    {
        return dynamic_cast<CPaintShotGun*>(m_pHost)->Get_State(0);
    }


//     if (m_fBehaviorTime >= 0.1f) {
//         // 재장전 시
//         if (ShotGun->m_fMaxBullet > 0 && ShotGun->m_fBulletCount < 6) {
//             _int bulletsToReload = min(6 - ShotGun->m_fBulletCount, ShotGun->m_fMaxBullet);
//             ShotGun->m_fBulletCount += bulletsToReload;
//             ShotGun->m_fMaxBullet -= bulletsToReload;
//             ShotGun->m_bIsBullet = true;
//         }
//         // 총알이 다 떨어진 경우
//         if (ShotGun->m_fMaxBullet == 0 && ShotGun->m_fBulletCount == 0) {
//             ShotGun->m_bIsBullet = false; // 총알이 다 떨어진 경우 발사 불가능
//         }
//     }
//
//     if (m_fBehaviorTime >= 0.2f) {
//         ShotGun->m_bReload = false;
//         return m_pHost->Get_State(0); // Idle
//     }
//     if (ShotGun->m_bReady) {
//         return m_pHost->Get_State(3); // Ready
//     }
    return nullptr;
}

void CPaintShotGun_Reload::Release(CPlayerGun* ShotGun)
{
	m_pHost->Reset_GunMoveDown();
	m_pHost->Reset_GunMoveRight();
    m_fBehaviorTime = 0.f;
	//ShotGun->m_fGunMoveRight = 3.f;
	//ShotGun->m_fGunMoveDown = 1.f;

    SoundMgr()->StopSound(SOUND_PLAYER2);
}
