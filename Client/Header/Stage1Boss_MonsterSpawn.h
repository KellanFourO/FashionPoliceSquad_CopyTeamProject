#pragma once
#include "Stage1BossState.h"
class CStage1Boss_MonsterSpawn : public CStage1BossState
{
public:
	CStage1Boss_MonsterSpawn();
	virtual ~CStage1Boss_MonsterSpawn();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster)override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster)override;

};
