#pragma once
#include "PlayerGunState.h"
class CPaintShotGun_Lazer : public CPlayerGunState
{
public:
	CPaintShotGun_Lazer();
	virtual ~CPaintShotGun_Lazer();

public:
	virtual void Initialize(CPlayerGun* _ShotGun) override;
	virtual CPlayerGunState* Update(CPlayerGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CPlayerGun* _ShotGun) override;

	_bool m_bAttack = false;

	_float m_fMoveRightSum = 0;
	_float m_fMoveDownSum = 0;
	_float m_fMoveRightMax = 0.f;
	_float m_fMoveDownMax = 0.f;

	_vec3			m_vPrePos;
};
