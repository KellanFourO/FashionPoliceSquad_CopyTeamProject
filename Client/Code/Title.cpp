#include "stdafx.h"
#include "Title.h"

#include "Export_System.h"
#include "Export_Utility.h"

CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTitle::CTitle(const CTitle& rhs)
	: Engine::CGameObject(rhs)
{
}

CTitle::~CTitle()
{
}

HRESULT CTitle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LogoTitleTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}


CTitle* CTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTitle* pInstance = new CTitle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Title Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTitle::Free()
{
	__super::Free();
}



HRESULT CTitle::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 400.0f;
	m_fSizeY = 250.0f;

	m_fX = 400.f;
	m_fY = 450.f;

	_vec3 vPos, vScale;

	vScale = { m_fSizeX * 0.5f, m_fSizeY * 0.5f, 1.f };

	vPos = { m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f };

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

Engine::_int CTitle::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CTitle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}
void CTitle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

}