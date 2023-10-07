#pragma once
#include "PlayerGunState.h"
class CTailorAssertRifle_Idle : public CPlayerGunState
{
public:
	CTailorAssertRifle_Idle();
	virtual ~CTailorAssertRifle_Idle();

public:
	virtual void Initialize(CPlayerGun* _Rifle) override;
	virtual CPlayerGunState* Update(CPlayerGun* Rifle, const float& fTimeDelta) override;
	virtual void Release(CPlayerGun* _Rifle) override;

private:
	_bool m_bAttack = false;
	_float m_fBehaviorTime = 0.f;

	_vec3			m_vPrePos;

};
