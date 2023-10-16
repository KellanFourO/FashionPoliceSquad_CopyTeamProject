#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CEffectTex;
class CTexture;
class CTransform;
class CCalculator;

class CJumpShockWaveEffect2 : public Engine::CGameObject
{
private:
	explicit CJumpShockWaveEffect2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJumpShockWaveEffect2(const CJumpShockWaveEffect2& rhs);
	virtual ~CJumpShockWaveEffect2();

public:
	virtual HRESULT			Ready_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;
	virtual void			Render_GameObject() override;

public:
	virtual void						OnCollisionEnter(CCollider* _pOther);
	virtual void						OnCollisionStay(CCollider* _pOther);
	virtual void						OnCollisionExit(CCollider* _pOther);
private:
	HRESULT					Add_Component();

private:
	_bool				m_bLateInit = true;
	CEffectTex*			m_pEffectBufferCom = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	_float				m_fLoop = 0.f;
	_float				m_fFrame = 0.f;
	//_float				m_fAngle = 0.f;
	_float				m_fDMG = 10.f;
	_float				m_fSpeed = 0.f;
	_float				m_fEffectDieTime = 0.f;



public:
	static CJumpShockWaveEffect2* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos);

private:
	virtual void Free() override;
};

END