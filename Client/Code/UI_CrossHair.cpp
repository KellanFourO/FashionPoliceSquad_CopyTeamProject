#include "stdafx.h"
#include "UI_CrossHair.h"
#include "UIMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"


CCrossHair::CCrossHair(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CCrossHair::CCrossHair(const CCrossHair& rhs)
	: CGameObject(rhs)
{
}

CCrossHair::~CCrossHair()
{
}



HRESULT Engine::CCrossHair::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTextureCom->Ready_Texture(TEX_NORMAL, L"../Bin/Resource/Texture/UI/RipleCrossHair.png",1);


	return S_OK;
}

Engine::_int Engine::CCrossHair::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	Mouse_Chase();

	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CCrossHair::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CCrossHair::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(m_iCrossHairIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CCrossHair::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CrossHairTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}


void CCrossHair::Sprite_Function(const _float& fTimeDelta)
{

// 	if (iCount < 10)
// 	{
// 		++iCount;
// 	}
// 	else
// 	{
// 		++iTempState;
// 		iCount = 0;
// 	}
//
// 	if (iTempState > 12)
// 		iTempState = 0;
//
//
//
// 	switch (iTempState)
// 	{
// 	case 0:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"SPRITE_hud_front.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 1:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_front_hurt.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 2:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_grimace_low.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 3:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_left.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 4:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_right.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 5:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smile.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 6:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smirk.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 7:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_sad_dirt.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 8:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_lef_hurt.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 9:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_grimace.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 10:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_left_hurt.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 11:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smirk_low.png")->Get_Info()->pTexture, 0);
// 		break;
//
// 	case 12:
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smile_low.png")->Get_Info()->pTexture, 0);
// 		break;
//
//
//
//
// 	}


}

void CCrossHair::Mouse_Chase()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	_vec3 vScale, vPos;
	_float fMulti = 2.f;

	switch (m_iCrossHairIndex)
	{
	case 0 :
		vScale = { 19.f * fMulti, 20.f * fMulti, 1.f };
		break;
	case 1:
		vScale = { 19.f * fMulti, 35.f * fMulti, 1.f };
		break;
	}

	vPos = { (_float)ptMouse.x - WINCX * 0.5f , -(_float)ptMouse.y + WINCY * 0.5f, 0.f };

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);


}


CCrossHair* CCrossHair::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossHair* pInstance = new CCrossHair(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CrossHair Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Engine::CCrossHair::Free()
{
	__super::Free();
}

