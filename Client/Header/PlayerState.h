#pragma once
#include "Engine_Typedef.h"
#include "Player.h"

class CPlayer;
class CPlayerState {
public:
			 CPlayerState() {};
	virtual ~CPlayerState() {};

public:
	virtual void Initialize(CPlayer* _Player) = 0;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) = 0;
	virtual void Release(CPlayer* _Player) = 0;


	CPlayer*		m_pHost;
	_float			m_fBehaviorTime = 0.f; // 행동 줄 시간

	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //애니메이션 관련

	_float			m_fAttackTime = 0;

	//PlayerStateID	StateID = PlayerStateID::Player_IDLE;
};