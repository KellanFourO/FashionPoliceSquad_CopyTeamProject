#pragma once
#include "KickBoardState.h"
class CKickBoard_IDLE : public CKickBoardState
{
public:
	CKickBoard_IDLE();
	virtual ~CKickBoard_IDLE();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

	

	_float m_fSpeed;
};
