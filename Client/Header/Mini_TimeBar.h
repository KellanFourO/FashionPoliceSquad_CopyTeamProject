#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_TimeBar :   public CGameObject
{
private:
	explicit			 CMini_TimeBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_TimeBar(const CMini_TimeBar& rhs);
	virtual				~CMini_TimeBar();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

	void				Set_TextureNumber(int _iTex) { m_iTextureIndex = _iTex; }
	_bool				Get_TimeOverCheck() { return m_bTimeOverCheck; }

private:
	HRESULT				Add_Component();
	void				Time_Count();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex;
	_bool					m_bTimeOverCheck = false;

	_vec3					m_vStartPos;
	_vec3					m_vStartScale;

	float					m_fTimeFullCount = 150;
	float					m_fTimeCount = 150;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;


public:
	static CMini_TimeBar* Create(LPDIRECT3DDEVICE9 pGraphicDev, int m_iTextureIndex);
private:
	virtual void		 Free();


};

