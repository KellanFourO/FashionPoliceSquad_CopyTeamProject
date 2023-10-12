#pragma once
#include "GameObject.h"



// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMyCardTexture : public Engine::CGameObject
{

private:
	explicit					 CMyCardTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CMyCardTexture(const CMyCardTexture& rhs);
	virtual						~CMyCardTexture();

public:
	CTexture*					Get_Texture() { return m_pTextureCardCom; };

public:
	virtual HRESULT				Ready_GameObject(_float StartX);
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta, _vec3& vPos, _float fAngle);
	virtual void				LateUpdate_GameObject() override;

private:
	HRESULT						Add_Component();

private:
	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureCardCom = nullptr;

	CTransform*					m_pTransformCom = nullptr;

	_matrix						m_matProj, m_matView;

	_float						m_fSpeed, m_fHorMultyply, m_fVerMultyply;

	_vec3						m_vPos, m_vScale;


public:
	static CMyCardTexture*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _float StartX);

private:
	virtual void				Free() override;
};

END


