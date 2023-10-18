#pragma once
#include "BossBigDaddyState.h"

class CBossBigDaddy_Dead : public CMonsterState
{
public:
	CBossBigDaddy_Dead();
	virtual ~CBossBigDaddy_Dead();

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
