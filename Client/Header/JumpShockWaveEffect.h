#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CEffectTex;
class CTexture;
class CTransform;
class CCalculator;

class CJumpShockWaveEffect : public Engine::CGameObject
{
private:
	explicit CJumpShockWaveEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJumpShockWaveEffect(const CJumpShockWaveEffect& rhs);
	virtual ~CJumpShockWaveEffect();

public:
	virtual HRESULT			Ready_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;
	virtual void			Render_GameObject() override;

private:
	HRESULT					Add_Component();

private:
	_bool				m_bLateInit = true;
	CEffectTex*			m_pEffectBufferCom = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	_float				m_fLoop = 0.f;
	_float				m_fFrame = 0.f;

	_float				m_fEffectDieTime = 0.f;

public:
	static CJumpShockWaveEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos);

private:
	virtual void Free() override;
};

END