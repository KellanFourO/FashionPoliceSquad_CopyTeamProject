#pragma once
#include "GameObject.h"
#include "Player.h"


// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CWeaponIcon : public Engine::CGameObject
{

private:
	explicit					 CWeaponIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CWeaponIcon(const CWeaponIcon& rhs);
	virtual						~CWeaponIcon();

public:
	virtual HRESULT				Ready_GameObject(UIDATA& _Info, _int _TextureIndex);
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

public:
	_bool						Get_Select() { m_bSelect;}
	void						Set_Select(_bool _bSelect) { m_bSelect = _bSelect;}

private:
	HRESULT						Add_Component();

private:
	_bool						m_bSelect = false;
	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	_int						m_iTextureIndex = 0;


	CTransform*					m_pTransformCom = nullptr;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;

	UIDATA						m_tInfo;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CWeaponIcon*			Create(LPDIRECT3DDEVICE9 pGraphicDev, UIDATA& _Info, _int _TextureIndex);

private:
	virtual void				Free() override;
};

END


