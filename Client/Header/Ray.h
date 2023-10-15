#pragma once
#include "GameObject.h"
#include "Engine_Define.h"

class CRay :public CGameObject
{
private:
	explicit CRay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRay(const CRay& rhs);
	virtual ~CRay();

public:
	virtual HRESULT						Ready_GameObject()override;
	virtual _int						Update_GameObject(const _float& fTimeDelta)override;
	virtual void						LateUpdate_GameObject()override;
	virtual void						Render_GameObject()override;
public:
	virtual void						OnCollisionEnter(CCollider* _pOther);
	virtual void						OnCollisionStay(CCollider* _pOther);
	virtual void						OnCollisionExit(CCollider* _pOther);
	_bool								PerformRaycast(_vec3 vStart, _vec3 vDir, _vec3& vHitPoint, _float fRayLength);

public:
	void								Set_Host(CGameObject* _pHost) { m_pHost = _pHost; }
	//void								Set_Player(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
public:
	CGameObject* m_pHost = nullptr;
	CGameObject* m_pColTarget = nullptr;
	CTransform* m_pPlayerTransform = nullptr;
	_vec3 m_vHitPoint;
public:
	static CRay* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT								Add_Component();
private:
	virtual void Free() override;
};

