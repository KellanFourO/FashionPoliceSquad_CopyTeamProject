#include "stdafx.h"
#include "UI_Portrait.h"
#include "UIMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"

CPortrait::CPortrait(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPortrait::CPortrait(const CPortrait& rhs)
	: Engine::CGameObject(rhs)
{
}

CPortrait::~CPortrait()
{
}

HRESULT CPortrait::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PortraitTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}


CPortrait* CPortrait::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPortrait* pInstance = new CPortrait(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Portrait Create Failed");
		return nullptr;
	}


	return pInstance;
}

void CPortrait::Free()
{
	__super::Free();
}



HRESULT CPortrait::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 0.2f;

	vPos = { 150.f, 50.f, 0.f };
	vScale = { 283.f * fMultiply, 251.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}

Engine::_int CPortrait::Update_GameObject(const _float& fTimeDelta)
{
	if (m_ePortrait != PORTRAITTAG::PORT_DES && m_ePortrait != PORTRAITTAG::PORT_DESNEON)
	{
		_vec3 vPos;
		vPos = { 600.f, 50.f, 0.f };
		vPos.x = vPos.x - WINCX * 0.5f;
		vPos.y = -vPos.y + WINCY * 0.5f;

		m_pTransformCom->Set_Pos(vPos);
	}
	else
	{
		_vec3 vPos;
		vPos = { 150.f, 50.f, 0.f };

		vPos.x = vPos.x - WINCX * 0.5f;
		vPos.y = -vPos.y + WINCY * 0.5f;

		m_pTransformCom->Set_Pos(vPos);
	}

	Engine::Add_RenderGroup(RENDER_UI, this);
	int iExit = __super::Update_GameObject(fTimeDelta);



	return iExit;
}

void CPortrait::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}
void CPortrait::Render_GameObject()
{


		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CPortrait::SetPortrait()
{
	switch (m_ePortrait)
	{
	case Engine::PORTRAITTAG::PORT_DES:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"portrait-des.png")->Get_Info()->pTexture, 0);
		break;
	case Engine::PORTRAITTAG::PORT_DESNEON:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"portrait-des-neon.png")->Get_Info()->pTexture, 0);
		break;
	case Engine::PORTRAITTAG::PORT_HALEY:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"portrait-haley.png")->Get_Info()->pTexture, 0);
		break;
	case Engine::PORTRAITTAG::PORT_DEEPCOAT:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"portrait-deepcoat.png")->Get_Info()->pTexture, 0);
		break;
	case Engine::PORTRAITTAG::PORT_BAUSS:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"portrait-bauss.png")->Get_Info()->pTexture, 0);
		break;
	case Engine::PORTRAITTAG::PORT_TURNCOAT:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"portrait-turncoat.png")->Get_Info()->pTexture, 0);
		break;
	}
}

