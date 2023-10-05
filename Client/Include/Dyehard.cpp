#include "stdafx.h"
#include "Dyehard.h"

#include "Export_System.h"
#include "Export_Utility.h"

CDyehard::CDyehard(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CDyehard::CDyehard(const CDyehard& rhs)
	:Engine::CGameObject(rhs)
{
}

CDyehard::~CDyehard()
{
}

HRESULT CDyehard::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CDyehard::Update_GameObject(const _float& fTimeDelta)
{
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"GameLogic", L"Player", L"Com_Transform"));
	NULL_CHECK_RETURN(pPlayerTransCom, -1);

	_vec3	vPlayerPos,vPlayerLook;

	pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	pPlayerTransCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook,&vPlayerLook);

	m_pTransformCom->Set_Pos(vPlayerPos+ vPlayerLook/10);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CDyehard::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDyehard::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDyehard::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Dyehard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CDyehard* CDyehard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDyehard* pInstance = new CDyehard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CDyehard::Free()
{
	__super::Free();
}
