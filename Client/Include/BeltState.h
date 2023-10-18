#pragma once
#include "Engine_Typedef.h"
#include "Belt.h"

class CBelt;
class CBeltState {
public:
	CBeltState() {};
	virtual ~CBeltState() {};

public:
	virtual void Initialize(CBelt* _Belt) = 0;
	virtual CBeltState* Update(CBelt* Belt, const float& fDetltaTime) = 0;
	virtual void Release(CBelt* _Belt) = 0;

	CBelt*			m_pHost = nullptr;
	_float			m_fBehaviorTime = 0.f; // 행동 줄 시간

	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //애니메이션 관련

	_float			m_fAttackTime = 0;

	_float			m_fChargeTime = 2.f;


};