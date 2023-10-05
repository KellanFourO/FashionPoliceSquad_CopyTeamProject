#pragma once
#include "PlayerState.h"
class CPlayer_Heal : public CPlayerState
{
public:
	CPlayer_Heal();
	virtual ~CPlayer_Heal();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};