#pragma once
#include "RifleState.h"
class CRifle_SHOT : public CRifleState
{
public:
	CRifle_SHOT();
	virtual ~CRifle_SHOT();

public:
	virtual void Initialize(CRifle* _Rifle) override;
	virtual CRifleState* Update(CRifle* Rifle, const float& fDetltaTime) override;
	virtual void Release(CRifle* _Rifle) override;

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
