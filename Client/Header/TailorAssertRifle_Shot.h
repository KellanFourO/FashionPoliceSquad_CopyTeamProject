#pragma once
#include "PlayerGunState.h"
class CTailorAssertRifle_Shot : public CPlayerGunState
{
public:
	CTailorAssertRifle_Shot();
	virtual ~CTailorAssertRifle_Shot();

public:
	virtual void Initialize(CPlayerGun* _Rifle) override;
	virtual CPlayerGunState* Update(CPlayerGun* Rifle, const float& fDetltaTime) override;
	virtual void Release(CPlayerGun* _Rifle) override;

private:
	_bool			m_bAttack = true;

	_float			 m_fMoveRightMax;
	_float			 m_fMoveUpMax;

	_float			 m_fMoveRightSum;
	_float			 m_fMoveDownSum;

	_float			 m_fScaleMax;
	_float			 m_fScaleReduce;

	_vec3			m_vPrePos;
	_vec3			m_vBaseScale;
};
