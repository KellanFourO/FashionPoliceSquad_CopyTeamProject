#pragma once
#include "Stage1BossState.h"
class CStage1Boss_Dead : public CStage1BossState
{
public:
	CStage1Boss_Dead();
	virtual ~CStage1Boss_Dead();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster)override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster)override;

	_float m_fTick = 0.0f;
};
