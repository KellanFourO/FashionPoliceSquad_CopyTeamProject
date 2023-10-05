#pragma once
#include "RifleState.h"
class CRifle_IDLE : public CRifleState
{
public:
	CRifle_IDLE();
	virtual ~CRifle_IDLE();

public:
	virtual void Initialize(CGun* _Rifle) override;
	virtual CRifleState* Update(CGun* Rifle, const float& fDetltaTime) override;
	virtual void Release(CGun* _Rifle) override;

	_bool m_bAttack = false;
	_float m_fBehaviorTime = 0.f;

	_vec3			m_vPrePos;

};
