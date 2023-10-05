#pragma once
#include "MonsterState.h"
class CDullSuitMonster_Attack : public CMonsterState
{
	enum ATTACKSTATE { READY1, READY2, THROW, THROWEND };

public:
			 CDullSuitMonster_Attack();
	virtual ~CDullSuitMonster_Attack();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;


private:
	_float		m_fTick = 0.f;
	ATTACKSTATE m_eAttack = READY1;
};
