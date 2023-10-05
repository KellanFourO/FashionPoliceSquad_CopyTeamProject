#pragma once
#include "ShotGunState.h"
class CShotGun_SHOT : public CShotGunState
{
public:
	CShotGun_SHOT();
	virtual ~CShotGun_SHOT();

public:
	virtual void Initialize(CGun* _ShotGun) override;
	virtual CShotGunState* Update(CGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CGun* _ShotGun) override;

	_bool m_bAttack = false;

	_float m_fMoveRightMax;
	_float m_fMoveUpMax;

	_float m_fMoveRightSum;
	_float m_fMoveDownSum;

	_float m_fScaleMax;
	_float m_fScaleReduce;

	_vec3			m_vPrePos;
	_vec3			m_vBaseScale;
};
