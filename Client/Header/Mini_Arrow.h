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
	explicit			 CMini_Arrow();
	explicit			 CMini_Arrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Arrow(const CMini_Arrow& rhs);
	virtual				~CMini_Arrow();

public:
	virtual HRESULT		Ready_GameObject(int iRandomNumber);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

	_vec3				Get_ArrowPos() { return m_vPos; }
	MINIGAME_ARROW_TYPE	Get_AroowType() { return m_eArrowType; }

private:
	HRESULT				Add_Component();

	void				Set_ArrowIndex(int iIndex) { m_iArrowIndex = iIndex; }
	void				Set_Rand_ArrowType(int iRandomNumber);


private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 0; //몇 번째 텍스쳐
	_int					m_iArrowIndex;

	_vec3					m_vPos;

	MINIGAME_ARROW_TYPE		m_eArrowType = MINIGAME_ARROW_TYPE::MINIGAME_ARROW_END;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;


public:
	static CMini_Arrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex, _int iRandomNumber);
private:
	virtual void		 Free();


};

