#pragma once
#include "MonsterState.h"

class CStage1BossState : public CMonsterState
{
public:
	CStage1BossState() {};
	virtual ~CStage1BossState() {};

public:
	virtual void Initialize(CMonster* _Monster) {};
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) { return nullptr; };
	virtual void LateUpdate(CMonster* _Monster) {};
	virtual void Release(CMonster* _Monster) {};
	virtual void Render(CMonster* _Monster) {};

public:
	BOSSPHASE			m_ePhase = BOSSPHASE::PHASE_1;

};