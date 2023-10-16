#pragma once

#include "Component.h"
#include "Base.h"
#include "Collider.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit							 CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit							 CGameObject(const CGameObject& rhs);
	virtual								~CGameObject();

public:
	// Get
	CComponent*							Get_Component(COMPONENTID eID, COMPONENTTAG eComponentTag);
	const OBJECTTAG&					Get_ObjectTag()	const { return m_eObjectTag; }
	_float								Get_ViewZ() { return m_fViewZ; }
	LPDIRECT3DDEVICE9					Get_GraphicDev() { return m_pGraphicDev; }
	_uint								Get_iIndex() { return m_iIndex; }
	CCollider*							Get_Collider() { return m_pCollider; }
	_bool								Get_Pause() { return m_bPause; }
	CTransform*							Get_Transform() { return m_pTransformCom; }
	BULLETTYPE							Get_HitType() { return m_eHitType; }
	_bool								Get_Dead() { return m_IsDead;}
	_float								Get_Angle() { return m_fAngle; }
	// Set
	void								Set_ObjectTag(OBJECTTAG _eTag) { m_eObjectTag = _eTag; }
	void								Set_Dead(_bool _IsDead) { m_IsDead = _IsDead; }
	void								Set_Pause(_bool _bPause) { m_bPause = _bPause; }
	void								Set_Angle(_float fAngle) { m_fAngle = fAngle; }


	const _bool&						IsDead() const { return m_IsDead; }
	void								Compute_ViewZ(const _vec3* pPos);

public:
	virtual HRESULT						Ready_GameObject();
	virtual _int						Update_GameObject(const _float& fTimeDelta);
	virtual void						LateUpdate_GameObject();
	virtual void						Render_GameObject()PURE;

// TODO - 준호 함수
public:
	virtual void						OnCollisionEnter(CCollider* _pOther) {};
	virtual void						OnCollisionStay(CCollider* _pOther) {};
	virtual void						OnCollisionExit(CCollider* _pOther) {};

protected:
	map<COMPONENTTAG, CComponent*>		m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	CCollider*							m_pCollider = nullptr;
	_float								m_fViewZ=0.f;
	_bool								m_IsDead = false;
	OBJECTTAG							m_eObjectTag=OBJECTTAG::OBJECT_END;
	BULLETTYPE							m_eHitType = BULLETTYPE::BULLETTYPE_END;


	_bool								m_bPause = false;
	_float								m_fAngle = 0.f;
	_vec3								m_vCamPos = {0.f,0.f,0.f};
public:
	_uint								m_iIndex = 100;
	CTransform*							m_pTransformCom = nullptr;
private:
	CComponent*							Find_Component(COMPONENTTAG eComponentTag, COMPONENTID eID);

public:
	virtual void						Free();
};

END