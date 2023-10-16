#pragma once
#include "PlayerState.h"
class CPlayer_Dash : public CPlayerState
{
public:
	CPlayer_Dash();
	virtual ~CPlayer_Dash();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};