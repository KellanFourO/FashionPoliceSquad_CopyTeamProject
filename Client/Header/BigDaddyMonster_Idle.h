#pragma once
#include "BigDaddyMonsterState.h"
class CBigDaddyMonster_Idle : public CMonsterState
{
public:
	CBigDaddyMonster_Idle();
	virtual ~CBigDaddyMonster_Idle();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;



};
