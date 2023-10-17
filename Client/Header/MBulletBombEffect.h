#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CEffectTex;
class CTexture;
class CTransform;
class CCalculator;

class CMBulletBombEffect : public Engine::CGameObject
{
private:
	explicit CMBulletBombEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMBulletBombEffect(const CMBulletBombEffect& rhs);
	virtual ~CMBulletBombEffect();

public:
	virtual void Render_GameObject() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);
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
	static CMBulletBombEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

END