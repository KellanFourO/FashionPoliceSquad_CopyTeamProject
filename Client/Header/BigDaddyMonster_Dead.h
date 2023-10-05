#pragma once
#include "BigDaddyMonsterState.h"
class CBigDaddyMonster_Dead : public CMonsterState
{
public:
	CBigDaddyMonster_Dead();
	virtual ~CBigDaddyMonster_Dead();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

	_float m_fBehaviorTime;
	
private:
	_float m_fTick;
	


	
};
