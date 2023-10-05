#include "stdafx.h"
#include "WireFrame.h"

#include "Export_System.h"
#include "Export_Utility.h"


CWireFrame::CWireFrame(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CWireFrame::CWireFrame(const CWireFrame& rhs)
	: Engine::CGameObject(rhs)
{
}

CWireFrame::~CWireFrame()
{
}

HRESULT CWireFrame::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CWireTex*>(Engine::Clone_Proto(L"Proto_WireTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_WireTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

CWireFrame* CWireFrame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWireFrame* pInstance = new CWireFrame(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("WireFrame Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWireFrame::Free()
{
	__super::Free();
}

HRESULT CWireFrame::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CWireFrame::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return __super::Update_GameObject(fTimeDelta);
}

void CWireFrame::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CWireFrame::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


