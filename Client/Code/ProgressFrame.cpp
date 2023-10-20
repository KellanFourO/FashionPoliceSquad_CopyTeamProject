#include "stdafx.h"
#include "ProgressFrame.h"

#include "Export_System.h"
#include "Export_Utility.h"

CProgressFrame::CProgressFrame(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CProgressFrame::CProgressFrame(const CProgressFrame& rhs)
	: Engine::CGameObject(rhs)
{
}

CProgressFrame::~CProgressFrame()
{
}

HRESULT CProgressFrame::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ProgressBarFrameTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);



	return S_OK;
}


CProgressFrame* CProgressFrame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProgressFrame* pInstance = new CProgressFrame(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("ProgressFrame Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProgressFrame::Free()
{
	__super::Free();
}



HRESULT CProgressFrame::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fXRatio = 6.f;
	_float fYRatio = 2.f;

	m_fSizeX = 128.0f * fXRatio;
	m_fSizeY = 16.0f * fYRatio;

	m_fX = 400.f;
	m_fY = 580.f;

	_vec3 vPos, vScale;

	vScale = { m_fSizeX * 0.5f, m_fSizeY * 0.5f, 1.f };

	vPos = { m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f };

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

Engine::_int CProgressFrame::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CProgressFrame::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}
void CProgressFrame::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();



}