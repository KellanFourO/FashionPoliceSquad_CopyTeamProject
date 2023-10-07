#pragma once
#include "PlayerGunState.h"
class CTailorAssertRifle_Ready : public CPlayerGunState
{
public:
	CTailorAssertRifle_Ready();
	virtual ~CTailorAssertRifle_Ready();

public:
	virtual void Initialize(CPlayerGun* _Rifle) override;
	virtual CPlayerGunState* Update(CPlayerGun* Rifle, const float& fDetltaTime) override;
	virtual void Release(CPlayerGun* _Rifle) override;

private:
	_bool			m_bAttack = false;
	_float			m_fMoveRightSum = -0.025f;
	_float			m_fMoveDownSum = -0.01f;
	_float			m_fMoveDownMax = 0.f;

	_vec3			m_vPrePos;
};
