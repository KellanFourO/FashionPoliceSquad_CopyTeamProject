#pragma once
#include "PlayerState.h"
class CPlayer_Left : public CPlayerState
{
public:
	CPlayer_Left();
	virtual ~CPlayer_Left();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};