#pragma once
#include "MonsterState.h"
#include "BossBigDaddy.h"
#include "BossBigDaddy2.h"
#include "BossBigDaddy3.h"
#include "BossBigDaddy4.h"

class CBossBigDaddyState : public CMonsterState
{
public:
	CBossBigDaddyState() {};
	virtual ~CBossBigDaddyState() {};


public:
	virtual void Initialize(CMonster* _Monster) {};
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) { return nullptr; };
	virtual void LateUpdate(CMonster* _Monster) {};
	virtual void Release(CMonster* _Monster) {};
	virtual void Render(CMonster* _Monster) {};
	

};