#include "..\..\Header\Component.h"
#include "GameObject.h"
CComponent::CComponent()
	: m_pGraphicDev(nullptr), m_bClone(false),m_pHost(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bClone(false), m_pHost(nullptr)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true), m_pHost(rhs.m_pHost)
{
	m_pGraphicDev->AddRef();
}


CComponent::~CComponent()
{
}

void CComponent::Init_Property(CGameObject* _pHost)
{
	m_pHost = _pHost;
	m_eObjectTag = m_pHost->Get_ObjectTag();

	if (m_eObjectTag == OBJECTTAG::OBJECT ||m_eObjectTag == OBJECTTAG::BUILD_CUBE||m_eObjectTag==OBJECTTAG::TERRAIN)
		m_pTransformCom = static_cast<CTransform*>(m_pHost->Get_Component(ID_STATIC,COMPONENTTAG::TRANSFORM ));
	else
		m_pTransformCom = static_cast<CTransform*>(m_pHost->Get_Component(ID_DYNAMIC,COMPONENTTAG::TRANSFORM ));
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
}
