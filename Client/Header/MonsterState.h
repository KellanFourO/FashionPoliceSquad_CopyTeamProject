#pragma once
#include "Engine_Typedef.h"
#include "Monster.h"
#include "BigDaddyMonster.h"
#include "DullSuitMonster.h"

class CMonsterState {
public:
	CMonsterState() {};
	virtual ~CMonsterState() {};

public:
	virtual void Initialize(CMonster* _Monster) = 0;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) = 0;
	virtual void LateUpdate(CMonster* _Monster) = 0;
	virtual void Release(CMonster* _Monster) = 0;
	virtual void Render(CMonster* _Monster) = 0;


	CMonster*		m_pHost;
	_float			m_fBehaviorTime = 0.f; // 행동 줄 시간
	_vec3			vPrePos;
	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //애니메이션 관련

	_float			m_fAttackTime = 0;

};