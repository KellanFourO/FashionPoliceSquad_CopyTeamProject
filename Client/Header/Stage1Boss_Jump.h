#pragma once
#include "Stage1BossState.h"
#include "Engine_Define.h"
class CStage1Boss_Jump : public CStage1BossState
{
public:
	CStage1Boss_Jump();
	virtual ~CStage1Boss_Jump();
	enum JUMPSTATE { JUMP_READY, JUMP_START, JUMP_END };

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster)override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster)override;

private:
	void				ShockWave();


private:

	_bool				m_bChange = false;
	int					m_iJumpStart = 2;
	int					m_iJumpEnd = 4;

	_vec3				vJumpPoint[4];

	_float				m_fTick = 0.f;
	_float				m_fJumpStaytime = 1.65f;
	_float				m_fJumpMoveTime = 1.2f;
	_float				m_fAgainTime = 2;
	_float				m_fJumpSpeed = 100.f;

	_bool				m_bEffect= true;
	_bool				m_bJump = false;
	JUMPSTATE			m_eJumpState = JUMP_READY;

	_int				m_iRandomIndex = 0;
	_int				m_iEnvIndex = 0;


	_float				m_fShockRadius = 128.f;


};
