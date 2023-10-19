#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END


class CMiniGameResult :   public CGameObject
{
private:
	explicit			 CMiniGameResult(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMiniGameResult(const CMiniGameResult& rhs);
	virtual				~CMiniGameResult();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

	void				Set_CallTime(_int _CallTime) { m_iCallTime = _CallTime * 60.f; }
	void				Set_Texture(_int TexNum) { m_iTextureIndex = TexNum; }

	void				Call(_int _Call_Time, _int TexNum);

private:
	HRESULT				Add_Component();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iCallTime = 0;
	_int					m_iTextureIndex = 0;
	_float					m_fSize;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;


public:
	static CMiniGameResult* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		 Free();


};

