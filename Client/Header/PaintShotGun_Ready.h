#pragma once
#include "PlayerGunState.h"
class CPaintShotGun_Ready : public CPlayerGunState
{
public:
	CPaintShotGun_Ready();
	virtual ~CPaintShotGun_Ready();

public:
	virtual void Initialize(CPlayerGun* _ShotGun) override;
	virtual CPlayerGunState* Update(CPlayerGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CPlayerGun* _ShotGun) override;

private:
	_bool			m_bAttack = false;
	_float			m_fMoveRightSum = -0.025f;
	_float			m_fMoveDownSum = -0.01f;
	_float			m_fMoveDownMax = 0.f;
	_vec3			m_vPrePos;
};
