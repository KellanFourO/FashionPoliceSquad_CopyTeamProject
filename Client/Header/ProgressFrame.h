#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CProgressFrame : public Engine::CGameObject
{
private:
	explicit CProgressFrame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CProgressFrame(const CProgressFrame& rhs);
	virtual ~CProgressFrame();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

public:
	void				Set_Ratio(_float _fRatio) { m_fValueRatio = _fRatio; }


private:
	HRESULT				Add_Component();

private:
	CUITex*					m_pBufferCom = nullptr;

	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;

	_float					m_fValueRatio = 0.f;

public:
	static CProgressFrame* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();


};

