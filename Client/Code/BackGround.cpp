#include "stdafx.h"
#include "..\Header\BackGround.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: Engine::CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{

	D3DXMatrixIdentity(&m_ViewMatrix);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 800.0f;
	m_fSizeY = 600.0f;

	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	_vec3 vPos, vScale;

	vScale = { m_fSizeX * 0.5f, m_fSizeY * 0.5f, 1.f };

	vPos = { m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f };

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround* pInstance = new CBackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}

Engine::_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CBackGround::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBackGround::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pBackTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

}



HRESULT CBackGround::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LogoCharacterTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pBackTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LogoBackTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CBackGround::Free()
{


	__super::Free();
}