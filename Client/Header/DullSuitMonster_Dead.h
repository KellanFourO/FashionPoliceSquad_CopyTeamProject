#pragma once
#include "MonsterState.h"
class CDullSuitMonster_Dead : public CMonsterState
{
public:
	CDullSuitMonster_Dead();
	virtual ~CDullSuitMonster_Dead();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

private:
	_float m_fTick = 0.f;
};
