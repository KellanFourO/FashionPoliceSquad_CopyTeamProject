#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Player.h"
#include "TailorAssertRifle.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CTailorAssertRifleHand : public CGameObject
{
private:
	explicit CTailorAssertRifleHand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTailorAssertRifleHand(const CTailorAssertRifleHand& rhs);
	virtual ~CTailorAssertRifleHand();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	HRESULT			Add_Component();
	void			Mouse_Input();

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	_bool					m_bLateInit = true;

	_float					m_fGunMoveRight;
	_float					m_fGunMoveDown;

	_vec3					m_vScale;

	CPlayer*				m_pHost;
	CTailorAssertRifle*		m_pRifle;
public:
	static CTailorAssertRifleHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

