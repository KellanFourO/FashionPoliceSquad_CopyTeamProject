#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CTitle : public Engine::CGameObject
{
private:
	explicit CTitle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTitle(const CTitle& rhs);
	virtual ~CTitle();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	HRESULT					Add_Component();

private:
	CUITex* m_pBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

private:
	_float		m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix		m_ViewMatrix, m_ProjMatrix;

public:
	static CTitle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();


};

