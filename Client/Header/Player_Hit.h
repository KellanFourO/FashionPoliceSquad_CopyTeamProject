#pragma once
#include "PlayerState.h"
class CPlayer_Hit : public CPlayerState
{
public:
	CPlayer_Hit();
	virtual ~CPlayer_Hit();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};