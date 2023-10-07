#pragma once
#include "MonsterState.h"
class CKickBoardMonster_Patrol : public CMonsterState
{


public:
			 CKickBoardMonster_Patrol();
	virtual ~CKickBoardMonster_Patrol();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

	void		ChangeDirection(const float& fDetltaTime);

private:
	_float		m_fTick = 0.f;
	_float		m_fRotateAngle = 90.f;
	_float		m_fMaxMoveDistance = 30.f;
	_float		m_fMoveDistance = 0.f;
	_float		m_fChangeTick = 0.f;

};
