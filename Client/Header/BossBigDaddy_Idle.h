#pragma once
#include "BigDaddyMonsterState.h"
class CBossBigDaddy_Idle : public CMonsterState
{
public:
	CBossBigDaddy_Idle();
	virtual ~CBossBigDaddy_Idle();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;



};
