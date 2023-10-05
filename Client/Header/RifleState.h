#pragma once
#include "Rifle.h"
#include "GunState.h"
class CGun;
class CRifleState : public CGunState
{
public:
	CRifleState() {};
	virtual ~CRifleState() {};
public:
	virtual void Initialize(CGun* _ShotGun) = 0;
	virtual CRifleState* Update(CGun* ShotGun, const float& fDetltaTime) = 0;
	virtual void Release(CGun* _ShotGun) = 0;

	_float m_fBehaviorTime = -0.f;
};

