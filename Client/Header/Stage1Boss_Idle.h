#pragma once
#include "Stage1BossState.h"
class CStage1Boss_Idle : public CStage1BossState
{

public:
	CStage1Boss_Idle();
	virtual ~CStage1Boss_Idle();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster)override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster)override;

private:
	int					m_iIdleStart = 2;
	int					m_iIdleEnd = 3;
	_float				m_fTick = 0.f;
	_float				m_fTestTick = 0.f;
	_bool				m_bStart = false;

};
