#pragma once
#include "GameObject.h"
#include "Player.h"
#include "UI_WeaponIcon.h"

// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CWeaponInfo : public Engine::CGameObject
{

private:
	explicit					 CWeaponInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CWeaponInfo(const CWeaponInfo& rhs);
	virtual						~CWeaponInfo();

public:
	virtual HRESULT				Ready_GameObject();
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

private:
	CPlayer*			 m_pPlayer;
	vector<CWeaponIcon*> m_vecWeaponIcon;
	_int				 m_iSelectIndex = 0;
	_bool				 m_bLateInit = true;
	//_int				 m_iPreIndex = 0;

public:
	static CWeaponInfo*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


