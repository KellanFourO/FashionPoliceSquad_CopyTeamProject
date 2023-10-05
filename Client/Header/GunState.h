#pragma once
#include "Engine_Typedef.h"
#include "Gun.h"


class CDyehard;
class CGunState{

public:
	CGunState() {};
	virtual ~CGunState() {};

public:
	virtual void Initialize(CGun* _ShotGun) = 0;
	virtual CGunState* Update(CGun* ShotGun, const float& fDetltaTime) = 0;
	virtual void Release(CGun* _ShotGun) = 0;

	_float			m_fBehaviorTime = 0.f; // �ൿ �� �ð�

	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //�ִϸ��̼� ����

	_float			m_fAttackTime = 0;


};