#pragma once
#include "BeltState.h"
class CBelt_IDLE : public CBeltState
{
public:
	CBelt_IDLE();
	virtual ~CBelt_IDLE();

public:
	virtual void Initialize(CBelt* _Belt) override;
	virtual CBeltState* Update(CBelt* Belt, const float& fDetltaTime) override;
	virtual void Release(CBelt* _Belt) override;

	_bool m_bAttack = false;

	//_vec3			m_vPrePos;

};
