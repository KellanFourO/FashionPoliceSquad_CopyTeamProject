#pragma once
#include "KickBoardMonsterState.h"
class CKickBoardMonster_Idle : public CKickBoardMonsterState
{
public:
	CKickBoardMonster_Idle();
	virtual ~CKickBoardMonster_Idle();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;


	_float m_fTick;
	_float m_fSpeed;
};
