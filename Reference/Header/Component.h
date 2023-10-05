#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual _int	Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void	LateUpdate_Component() {}
public:
	void			Set_Transform(CTransform* _pTransformCom)	{ m_pTransformCom = _pTransformCom; }
	void			Set_ObjectTag(OBJECTTAG _eObject)		{ m_eObjectTag = _eObject; }

	void			Init_Property(CGameObject* _pHost);
public:
	CTransform*		Get_Transform() { return m_pTransformCom; }
	OBJECTTAG		Get_ObjectTag() { return m_eObjectTag; }
	CGameObject*	Get_Host()		{ return m_pHost; }
	void			Set_Host(CGameObject* _Host) { m_pHost = _Host; }
protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bClone;

	CGameObject*				m_pHost;
	OBJECTTAG					m_eObjectTag;
	CTransform*					m_pTransformCom;
public:
	virtual CComponent* Clone()PURE;
	virtual void			Free();
};

END