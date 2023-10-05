#pragma once
#include "PlayerState.h"
class CPlayer_Jump : public CPlayerState
{
public:
	CPlayer_Jump();
	virtual ~CPlayer_Jump();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};