#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CEffectTex;
class CTexture;
class CTransform;

class CDashEffect : public Engine::CGameObject
{
private:
	explicit CDashEffect(LPDIRECT3DDEVICE9 pGarphicDev);
	explicit CDashEffect(const CDashEffect& rhs);
	virtual ~CDashEffect();

public:
	virtual HRESULT			Ready_GameObject()override;
	virtual void			Render_GameObject()override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;

private:
	HRESULT					Add_Component();
public:
	CEffectTex*		m_pEffectBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pPlayerTransform = nullptr;
	_float			m_fFrame = 0.f;
	_float			m_fAngle = 0.f;
	_matrix			m_matProj, m_matView;

	_float			m_DashEffectDieTime = 0.f;
public:
	static CDashEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

END
