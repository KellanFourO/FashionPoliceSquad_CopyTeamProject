#pragma once
#include "BeltState.h"
class CBelt_Ready : public CBeltState
{
public:
	CBelt_Ready();
	virtual ~CBelt_Ready();

public:
	virtual void Initialize(CBelt* _Belt) override;
	virtual CBeltState* Update(CBelt* Belt, const float& fDetltaTime) override;
	virtual void Release(CBelt* _Belt) override;

private:
	_bool	RopeActionTest();

	_bool m_bAttack = false;

	_float m_fMoveRightSum;
	_float m_fMoveDownSum;
	_float m_fRotationMax;
	_float m_fRotate;

	//_vec3			m_vPrePos;

};
