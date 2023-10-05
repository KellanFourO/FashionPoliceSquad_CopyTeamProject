#pragma once
#include "MonsterState.h"
class CDullSuitMonster_Chase : public CMonsterState
{


public:
			 CDullSuitMonster_Chase();
	virtual ~CDullSuitMonster_Chase();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;


private:
	_float		m_fTick = 0.f;
	
};
