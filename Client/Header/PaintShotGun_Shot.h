#pragma once
#include "PlayerGunState.h"
class CPaintShotGun_Shot : public CPlayerGunState
{
public:
	CPaintShotGun_Shot();
	virtual ~CPaintShotGun_Shot();

public:
	virtual void Initialize(CPlayerGun* _ShotGun) override;
	virtual CPlayerGunState* Update(CPlayerGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CPlayerGun* _ShotGun) override;

private:
	_bool	m_bAttack = true;

	_float	m_fMoveRightMax;
	_float	m_fMoveUpMax;

	_float	m_fMoveRightSum;
	_float	m_fMoveDownSum;

	_float	m_fScaleMax;
	_float	m_fScaleReduce;

	_vec3	m_vPrePos;
	_vec3	m_vBaseScale;
};
