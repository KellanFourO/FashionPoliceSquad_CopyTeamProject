#pragma once
#include "Engine_Typedef.h"
#include "Monster.h"
#include "BigDaddyMonster.h"
#include "DullSuitMonster.h"

class CMonsterState {
public:
	CMonsterState() {};
	virtual ~CMonsterState() {};

public:
	virtual void Initialize(CMonster* _Monster) = 0;
	virtual CMonsterState* Update(CMonster* Monster, const float& fDetltaTime) = 0;
	virtual void LateUpdate(CMonster* _Monster) = 0;
	virtual void Release(CMonster* _Monster) = 0;
	virtual void Render(CMonster* _Monster) = 0;


	CMonster*		m_pHost;
	_float			m_fBehaviorTime = 0.f; // �ൿ �� �ð�
	_vec3			vPrePos;
	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //�ִϸ��̼� ����

	_float			m_fAttackTime = 0;

};