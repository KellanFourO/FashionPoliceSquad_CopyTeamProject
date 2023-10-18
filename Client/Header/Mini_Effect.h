#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_Effect :   public CGameObject
{
private:
	explicit			 CMini_Effect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Effect(const CMini_Effect& rhs);
	virtual				~CMini_Effect();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

private:
	HRESULT				Add_Component();

public:
	void				Set_Move();
	void				Set_Dir();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 1;

	_vec3					m_PlayerPos;
	_vec3					m_PlayerScale;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;

public:
	static CMini_Effect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		 Free();


};

