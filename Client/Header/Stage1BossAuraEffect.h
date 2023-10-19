#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CSYTex;
class CTexture;
class CTransform;
class CCalculator;

class CStage1BossAuraEffect : public Engine::CGameObject
{
private:
	explicit CStage1BossAuraEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStage1BossAuraEffect(const CStage1BossAuraEffect& rhs);
	virtual ~CStage1BossAuraEffect();

public:
	virtual HRESULT			Ready_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;
	virtual void			Render_GameObject() override;

private:
	HRESULT					Add_Component();

private:
	_bool				m_bLateInit = true;
	CSYTex*				m_pBufferCom = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	_float				m_fFrame = 0.f;
	_int				m_iTextureIndex = 1;


	CGameObject*		m_pHost = nullptr;

public:
	static CStage1BossAuraEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pHost);

private:
	virtual void Free() override;
};

END