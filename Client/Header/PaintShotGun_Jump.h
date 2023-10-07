#pragma once
#include "PlayerGunState.h"
class CPaintShotGun_Jump : public CPlayerGunState
{
public:
	CPaintShotGun_Jump();
	virtual ~CPaintShotGun_Jump();

public:
	virtual void Initialize(CPlayerGun* _ShotGun) override;
	virtual CPlayerGunState* Update(CPlayerGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CPlayerGun* _ShotGun) override;

private:
	_bool m_bAttack = false;

	_float m_fMoveRightSum = -0.025f;
	_float m_fMoveDownSum = -0.01f;

	_vec3			m_vPrePos;
};
