#pragma once
#include "Engine_Typedef.h"
#include "Monster.h"
#include "BigDaddyMonster.h"
#include "DullSuitMonster.h"
#include "KickBoardMonster.h"
#include "Stage1Boss.h"

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

	_float	Get_CurFrame() { return m_fCurFrame; }
	_float	Get_MaxFrame() { return m_fMaxFrame; }
	_int	Get_Hor() { return m_iHor; }
	_int	Get_Ver() { return m_iVer; }
	_int	Get_TextureIndex() { return m_iTextureIndex; }

	CMonster*		m_pHost = nullptr;
	_float			m_fBehaviorTime = 0.f; // 행동 줄 시간
	_vec3			vPrePos = { 0.f,0.f,0.f};
	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_int			m_iVer = 0;
	_int			m_iHor = 0;
	_float			m_fAnimateTime = 0;   //애니메이션 관련
	_int			m_iTextureIndex = 0;
	_float			m_fAttackTime = 0;

};