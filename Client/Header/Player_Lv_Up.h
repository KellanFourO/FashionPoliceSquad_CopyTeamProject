#pragma once
#include "PlayerState.h"
class CPlayer_Lv_Up : public CPlayerState
{
public:
	CPlayer_Lv_Up();
	virtual ~CPlayer_Lv_Up();

public:
	virtual void Initialize(CPlayer* _Player) override;
	virtual CPlayerState* Update(CPlayer* Player, const float& fDetltaTime) override;
	virtual void Release(CPlayer* _Player) override;

};