#pragma once
#include "ShotGunState.h"
class CShotGun_JUMP : public CShotGunState
{
public:
	CShotGun_JUMP();
	virtual ~CShotGun_JUMP();

public:
	virtual void Initialize(CGun* _ShotGun) override;
	virtual CShotGunState* Update(CGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CGun* _ShotGun) override;

	_bool m_bAttack = false;

	_float m_fMoveRightSum = -0.025f;
	_float m_fMoveDownSum = -0.01f;

	_vec3			m_vPrePos;
};
