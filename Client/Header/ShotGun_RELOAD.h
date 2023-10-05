#pragma once
#include "ShotGunState.h"
class CShotGun_RELOAD : public CShotGunState
{
public:
	CShotGun_RELOAD();
	virtual ~CShotGun_RELOAD();

public:
	virtual void Initialize(CGun* _ShotGun) override;
	virtual CShotGunState* Update(CGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CGun* _ShotGun) override;

	_bool m_bAttack = false;

	_float m_fMoveRightSum = -0.025f;
	_float m_fMoveDownSum = -0.01f;
	_float m_fMoveDownMax = 0.f;

	_vec3			m_vPrePos;
};
