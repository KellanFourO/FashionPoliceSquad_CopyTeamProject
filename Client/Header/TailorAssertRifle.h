#pragma once

#include "Base.h"
#include "GameObject.h"
#include "PlayerGun.h"

class CGunState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CTailorAssertRifle : public CPlayerGun
{
private:
	explicit CTailorAssertRifle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTailorAssertRifle(const CTailorAssertRifle& rhs);
	virtual ~CTailorAssertRifle();

	enum ASSERTRIFLESTATE { IDLE, WALK, JUMP, READY, SHOT, RELOAD, BOMB, ASSERTRIFLESTATE_END };

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	ReadyState();

public:
	CPlayerGunState* Get_State(_int _index) { return m_pStateArray[_index]; }

private:
	HRESULT			Add_Component();

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	CPlayerGunState*		m_pStateArray[ASSERTRIFLESTATE_END];

public:
	static CTailorAssertRifle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer);

private:
	virtual void Free();
};

