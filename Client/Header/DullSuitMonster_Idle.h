#pragma once
#include "DullSuitMonsterState.h"
class CDullSuitMonster_Idle : public CDullSuitMonsterState
{
public:
	CDullSuitMonster_Idle();
	virtual ~CDullSuitMonster_Idle();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

};
