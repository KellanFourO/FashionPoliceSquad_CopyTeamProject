#pragma once
#include "ShotGunState.h"
class CShotGun_IDLE : public CShotGunState
{
public:
	CShotGun_IDLE();
	virtual ~CShotGun_IDLE();

public:
	virtual void Initialize(CGun* _ShotGun) override;
	virtual CShotGunState* Update(CGun* ShotGun, const float& fDetltaTime) override;
	virtual void Release(CGun* _ShotGun) override;

	_bool m_bAttack = false;

	_vec3			m_vPrePos;

};
