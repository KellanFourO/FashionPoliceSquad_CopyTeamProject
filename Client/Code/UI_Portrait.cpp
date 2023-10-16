#include "stdafx.h"
#include "UI_Portrait.h"

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

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-des-neon.png", 1);	//1
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-haley.png", 1);		//2
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-deepcoat.png", 1);	//3
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-bauss.png", 1);		//4
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-turncoat.png", 1);	//5
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-des-dull.png", 1);	//6
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-mystery.png", 1);	//7
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-phone.png", 1);		//8


	return S_OK;
}

Engine::_int CPortrait::Update_GameObject(const _float& fTimeDelta)
{

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

		m_pTextureCom->Render_Textrue(m_iTextureIndex);
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
		m_iTextureIndex = 0;
		break;
	case Engine::PORTRAITTAG::PORT_DESNEON:
		m_iTextureIndex = 1;
		break;
	case Engine::PORTRAITTAG::PORT_HALEY:
		m_iTextureIndex = 2;
		break;
	case Engine::PORTRAITTAG::PORT_DEEPCOAT:
		m_iTextureIndex = 3;
		break;
	case Engine::PORTRAITTAG::PORT_BAUSS:
		m_iTextureIndex = 4;
		break;
	case Engine::PORTRAITTAG::PORT_TURNCOAT:
		m_iTextureIndex = 5;
		break;
	case Engine::PORTRAITTAG::PORT_DESDULL:
		m_iTextureIndex = 6;
		break;
	case Engine::PORTRAITTAG::PORT_MYSTERY:
		m_iTextureIndex = 7;
		break;
	case Engine::PORTRAITTAG::PORT_PHONE:
		m_iTextureIndex = 8;
		break;
	}


}

