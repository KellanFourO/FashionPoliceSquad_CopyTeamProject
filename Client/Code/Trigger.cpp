
#include "stdafx.h"
#include "Trigger.h"

#include "ImGuiManager.h"

#include "Export_System.h"
#include "Export_Utility.h"


CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	: CGameObject(rhs)
{
}

CTrigger::~CTrigger()
{
}

HRESULT CTrigger::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCubeCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::CALCULATOR, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);
	//ID_STATIC이면 Collider -> Get_ID 에서 터짐!! -유진

	return S_OK;
}

HRESULT CTrigger::Ready_GameObject(_vec3 pMouse_Pos, _vec3 Size)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::O_TRIGGER);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	m_pTransformCom->Set_Scale(Size);
	m_pTransformCom->Set_Pos(pMouse_Pos);

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], m_pTransformCom->m_vScale);

	return S_OK;
}

_int CTrigger::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CTrigger::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CTrigger::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pCollider->Render_Collider();
	//m_pBufferCubeCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, _uint iIndex, 
	_vec3 Size, TRIGGER_CASE eCase, TRIGGER_TYPE eType, TRIGGER_NUMBER eTrName)
{
	CTrigger* pInstance = new CTrigger(pGraphicDev);

	pInstance->Set_TR_CASE(eCase);
	pInstance->Set_TR_TYPE(eType);
	pInstance->Set_TR_NUMBER(eTrName);
	pInstance->Set_IndexNumber(iIndex);

	if (FAILED(pInstance->Ready_GameObject(pMouse_Pos, Size)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Trigger Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();
}
