#pragma once
#include "PlayerState.h"
class CPlayer_LevelUp : public CPlayerState
{
public:
	CPlayer_LevelUp();
	virtual ~CPlayer_LevelUp();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};