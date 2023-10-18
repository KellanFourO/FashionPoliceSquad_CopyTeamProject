#pragma once
#include "BeltState.h"
class CBelt_ChargeAttack : public CBeltState
{
public:
	CBelt_ChargeAttack();
	virtual ~CBelt_ChargeAttack();

public:
	virtual void Initialize(CBelt* _Belt) override;
	virtual CBeltState* Update(CBelt* Belt, const float& fDetltaTime) override;
	virtual void Release(CBelt* _Belt) override;

private:
	_bool TargetStateChange();


public:

	_bool m_bAttack = false;

	_float m_fMoveRightSum;
	_float m_fMoveDownSum;
	_float m_fMoveRightMax;
	_float m_fMoveUpMax;

	_float m_fRotateMax;
	_float 	m_fRotate;

	_vec3			m_vPrePos;

};
