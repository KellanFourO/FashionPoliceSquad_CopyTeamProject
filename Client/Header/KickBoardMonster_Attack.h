#pragma once
#include "MonsterState.h"
class CKickBoardMonster_Attack : public CMonsterState
{
	enum ATTACKSTATE { READY, JUMP, JUMPEND };

public:
			 CKickBoardMonster_Attack();
	virtual ~CKickBoardMonster_Attack();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;


private:
	_float		m_fTick = 0.f;
	ATTACKSTATE m_eAttack = READY;
};
