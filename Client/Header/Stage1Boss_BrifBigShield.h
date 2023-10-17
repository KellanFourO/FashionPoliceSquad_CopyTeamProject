#pragma once
#include "Stage1BossState.h"
class CStage1Boss_BrifBigShield : public CStage1BossState
{
public:
	CStage1Boss_BrifBigShield();
	virtual ~CStage1Boss_BrifBigShield();
	enum BossDaddyDie { ALL_LIVE, LEFT_LIVE, RIGHT_LIVE, ALL_DEAD, BOSSDADDYDIE_END };

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster)override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster)override;

public:
	static bool		m_bcheck1;
	static bool		m_bcheck2;
	static bool		m_bcheck3;
	static bool		m_bcheck4;
	static bool		m_bLeftDead;
	static bool		m_bRightDead;
	static bool		m_bAllDead;
private:
	BossDaddyDie		m_eBossDaddyDie;

	_float				m_fTick=0.f;
	_float				m_fAgainTime = 2.f;
};

