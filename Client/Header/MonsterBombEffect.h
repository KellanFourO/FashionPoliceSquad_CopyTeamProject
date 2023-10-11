#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CEffectTex;
class CTexture;
class CTransform;
class CCalculator;

class CMonsterBombEffect : public Engine::CGameObject
{
private:
	explicit CMonsterBombEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBombEffect(const CMonsterBombEffect& rhs);
	virtual ~CMonsterBombEffect();

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
	static CMonsterBombEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

END