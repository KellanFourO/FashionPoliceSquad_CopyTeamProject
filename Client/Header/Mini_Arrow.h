#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_Arrow :   public CGameObject
{
private:
	explicit			 CMini_Arrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Arrow(const CMini_Arrow& rhs);
	virtual				~CMini_Arrow();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

private:
	HRESULT				Add_Component();


private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 1;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;


public:
	static CMini_Arrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		 Free();


};

