#pragma once
#include "MonsterState.h"
class CKickBoardMonster_Dead : public CMonsterState
{
public:
	CKickBoardMonster_Dead();
	virtual ~CKickBoardMonster_Dead();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

private:
	_float m_fTick = 0.f;
};
