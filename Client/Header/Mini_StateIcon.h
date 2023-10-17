#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_StateIcon :   public CGameObject
{
private:
	explicit			 CMini_StateIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_StateIcon(const CMini_StateIcon& rhs);
	virtual				~CMini_StateIcon();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;
	
	HRESULT				Change_State(_int m_iTex);

private:
	HRESULT				Add_Component();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTexIndex = 0;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;

	_bool					m_ClearCheck = false;

public:
	static CMini_StateIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		 Free();


};

