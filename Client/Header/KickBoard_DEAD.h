#pragma once
#include "MonsterState.h"
class CKickBoard_DEAD : public CMonsterState
{
public:
	CKickBoard_DEAD();
	virtual ~CKickBoard_DEAD();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

	_float m_fBehaviorTime;
};
