#pragma once

#include "Base.h"
#include "PlayerGun.h"
#include "MuzzlefFlash_Rifle.h"

class CPlayerGunState;

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

	enum ASSERTRIFLESTATE { IDLE, WALK, READY, SHOT, RELOAD, ASSERTRIFLESTATE_END };

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	ReadyState();

public:
	CPlayerGunState*	Get_State(_int _index) { return m_pStateArray[_index]; }
	CMuzzleFlash_Rifle* Get_Flash() { return m_pFlash; }

	void				Set_Flash(CMuzzleFlash_Rifle* _pFlash) { m_pFlash = _pFlash; }
	void				Set_Index(_int _iIndex) { m_iTextureIndex = _iIndex; }
private:
	HRESULT			Add_Component();

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	_int					m_iTextureIndex = 0;

	CMuzzleFlash_Rifle*		m_pFlash = nullptr; // √— ¿Ã∆Â∆Æ
	CPlayerGunState*		m_pStateArray[ASSERTRIFLESTATE_END];

public:
	static CTailorAssertRifle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer);

private:
	virtual void Free();
};

