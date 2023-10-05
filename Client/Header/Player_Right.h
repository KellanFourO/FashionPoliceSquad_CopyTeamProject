#pragma once
#include "PlayerState.h"
class CPlayer_Right : public CPlayerState
{
public:
	CPlayer_Right();
	virtual ~CPlayer_Right();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};