#pragma once
#include "Engine_Typedef.h"
#include "PlayerGun.h"

class CPlayerGunState{

public:
	CPlayerGunState() {};
	virtual ~CPlayerGunState() {};

public:
	virtual void Initialize(CPlayerGun* _PlayerGun) = 0;
	virtual CPlayerGunState* Update(CPlayerGun* _PlayerGun, const float& fTimeDelta) = 0;
	virtual void Release(CPlayerGun* _PlayerGun) = 0;

	_float			m_fBehaviorTime = 0.f; // 행동 줄 시간

	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //애니메이션 관련

	_float			m_fAttackTime = 0;


};