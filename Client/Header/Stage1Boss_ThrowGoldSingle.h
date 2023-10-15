#pragma once
#include "Stage1BossState.h"
#include "Bullet_GoldBar.h"

class CStage1Boss_ThrowGoldSingle : public CStage1BossState
{
	enum THROWSTATE { THROW, THROWREADY};
public:
	CStage1Boss_ThrowGoldSingle();
	virtual ~CStage1Boss_ThrowGoldSingle();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster)override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster)override;

private:
	void				Targeting();
	void				LoadBullet();

private:
	_bool					m_bChange = false;
	_int					m_iSingleThrowStart = 5;
	_int					m_iSingleThrowEnd = 1;

	_float					m_fTick = 0.f;


	_float					m_fThrowSpeed;
	_float					m_fBulletSpeed;
	_int					m_iBulletCount;
	_int					m_iThrowCount;

	_int					m_iCountTick;




	_vec3					m_vTargetDir;

	_bool					m_bThrow = false;

	THROWSTATE				m_eThrowstate = THROWREADY;
};
