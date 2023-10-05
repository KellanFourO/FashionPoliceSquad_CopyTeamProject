#pragma once
#include "PlayerState.h"
class CPlayer_Armor : public CPlayerState
{
public:
	CPlayer_Armor();
	virtual ~CPlayer_Armor();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};