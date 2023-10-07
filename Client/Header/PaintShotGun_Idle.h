#pragma once
#include "PlayerGunState.h"
class CPaintShotGun_Idle : public CPlayerGunState
{
public:
	CPaintShotGun_Idle();
	virtual ~CPaintShotGun_Idle();

public:
	virtual void Initialize(CPlayerGun* _ShotGun) override;
	virtual CPlayerGunState* Update(CPlayerGun* ShotGun, const float& fTimeDelta) override;
	virtual void Release(CPlayerGun* _ShotGun) override;

	_bool m_bAttack = false;

	_vec3			m_vPrePos;

};
