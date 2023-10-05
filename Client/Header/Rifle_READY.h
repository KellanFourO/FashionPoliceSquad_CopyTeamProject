#pragma once
#include "RifleState.h"
class CRifle_READY : public CRifleState
{
public:
	CRifle_READY();
	virtual ~CRifle_READY();

public:
	virtual void Initialize(CGun* _Rifle) override;
	virtual CRifleState* Update(CGun* Rifle, const float& fDetltaTime) override;
	virtual void Release(CGun* _Rifle) override;

	_bool m_bAttack = false;

	_float m_fMoveRightSum = -0.025f;
	_float m_fMoveDownSum = -0.01f;
	_float m_fMoveDownMax = 0.f;

	_vec3			m_vPrePos;
};
