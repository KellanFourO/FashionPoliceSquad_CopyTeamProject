#pragma once
#include "PlayerState.h"
class CPlayer_IDLE : public CPlayerState
{
public:
	CPlayer_IDLE();
	virtual ~CPlayer_IDLE();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};