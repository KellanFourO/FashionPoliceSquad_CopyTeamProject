#pragma once
#include "MonsterState.h"


class CDullSuitMonsterState : public CMonsterState
{
public:
	CDullSuitMonsterState() {};
	virtual ~CDullSuitMonsterState() {};

public:
	virtual void Initialize(CMonster* _Monster) {};
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) { return nullptr; };
	virtual void LateUpdate(CMonster* _Monster) {};
	virtual void Release(CMonster* _Monster) {};
	virtual void Render(CMonster* _Monster) {};

public:
	_vec3 vPrePos;

	// CMonsterState을(를) 통해 상속됨

};