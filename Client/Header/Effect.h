#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CEffectTex;
class CTexture;
class CTransform;
class CCalculator;

class CEffect : public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();

public:
	virtual void Render_GameObject() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

private:
	HRESULT					Add_Component();

private:
	CEffectTex* m_pEffectBufferCom = nullptr;
	//CTransform* m_pTransformCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;

	_float				m_fFrame = 0.f;

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

END