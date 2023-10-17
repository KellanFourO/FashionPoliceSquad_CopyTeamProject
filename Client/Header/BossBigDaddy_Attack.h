#pragma once
#include "MonsterState.h"
#include "BrifCase.h"
class CBossBigDaddy_Attack : public CMonsterState
{
	enum ATTACKSTATE { READY1, READY2, THROW, THROWEND };

public:
			 CBossBigDaddy_Attack();
	virtual ~CBossBigDaddy_Attack();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

private:
	CBullet* LoadBullet();

private:
	_float		m_fTick = 0.f;
	ATTACKSTATE m_eAttack = READY1;

};
