#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CSYRay : public Engine::CGameObject
{
private:
	explicit CSYRay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSYRay(const CSYRay& rhs);
	virtual ~CSYRay();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	static RAY		GetRay(LPDIRECT3DDEVICE9 _pGraphicDev);
	static RAY		GetSYRay(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pGameObject);

	CGameObject*    GetTarget() { return m_pTarget;}

private:
	_bool		m_bLateInit = true;
	CTransform* m_pPlayerTransform = nullptr;
	_float		m_fRayLength = 70.0f;
	RAY			m_tRAY;
	CGameObject* m_pHost = nullptr;
	CGameObject* m_pTarget = nullptr;

public:
	static CSYRay* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pHost);

private:
	HRESULT								Add_Component();
private:
	virtual void Free();


};

