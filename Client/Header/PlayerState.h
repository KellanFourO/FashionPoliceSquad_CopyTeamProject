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
	_float			m_fBehaviorTime = 0.f; // �ൿ �� �ð�

	_float			m_fCurFrame = 0.f;
	_float			m_fMinFrame = 1.f;
	_float			m_fMaxFrame = 1.f;
	_float			m_fAnimateTime = 0;   //�ִϸ��̼� ����

	_float			m_fAttackTime = 0;

	//PlayerStateID	StateID = PlayerStateID::Player_IDLE;
};