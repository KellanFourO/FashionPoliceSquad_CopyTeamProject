#pragma once
#include "BeltState.h"
class CBelt_CHARGE : public CBeltState
{
public:
	CBelt_CHARGE();
	virtual ~CBelt_CHARGE();

public:
	virtual void Initialize(CBelt* _Belt) override;
	virtual CBeltState* Update(CBelt* Belt, const float& fDetltaTime) override;
	virtual void Release(CBelt* _Belt) override;

	_bool m_bAttack = false;



	_vec3			m_vPrePos;

};
