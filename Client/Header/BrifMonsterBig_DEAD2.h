#pragma once
#include "BrifMonsterBigState.h"
class CBrifMonsterBig_DEAD2 : public CBrifMonsterBigState
{
public:
	CBrifMonsterBig_DEAD2();
	virtual ~CBrifMonsterBig_DEAD2();

public:
	virtual void Initialize(CMonster* _Monster) override;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) override;
	virtual void LateUpdate(CMonster* _Monster) override;
	virtual void Release(CMonster* _Monster)override;
	virtual void Render(CMonster* _Monster) override;

};
