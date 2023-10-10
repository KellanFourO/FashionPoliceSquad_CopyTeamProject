#include "..\..\Header\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
	Free();
}
 
CComponent* CGameObject::Find_Component(COMPONENTTAG eComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_FinderEnum<COMPONENTTAG>(eComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

CComponent* Engine::CGameObject::Get_Component(COMPONENTID eID, COMPONENTTAG eComponentTag)
{
	CComponent* pComponent = Find_Component(eComponentTag, eID);


	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

void Engine::CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	_vec3	vCamPos;
	m_vCamPos = (vCamPos - *pPos);
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&m_vCamPos);
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bPause)
	{
		for (auto& iter : m_mapComponent[ID_DYNAMIC])
			iter.second->Update_Component(fTimeDelta);
	}


	return m_IsDead;
}

void CGameObject::LateUpdate_GameObject()
{
	if (!m_bPause)
	{
		for (auto& iter : m_mapComponent[ID_DYNAMIC])
			iter.second->LateUpdate_Component();
	}
}

void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}
