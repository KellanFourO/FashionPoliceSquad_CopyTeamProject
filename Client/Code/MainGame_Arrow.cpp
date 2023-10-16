#include "stdafx.h"
#include "MainGame_Arrow.h"
#include "EventMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMainGame_Arrow::CMainGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMainGame_Arrow::CMainGame_Arrow(const CMainGame_Arrow& rhs)
	: Engine::CGameObject(rhs)
{
}

CMainGame_Arrow::~CMainGame_Arrow()
{
}



HRESULT CMainGame_Arrow::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pStateIcon = CMini_StateIcon::Create(m_pGraphicDev);
	//m_pArrow = CMini_Arrow::Create(m_pGraphicDev);
	//m_pTimeBar = CMini_TimeBar::Create(m_pGraphicDev);
	//m_pCursor = CMini_Cursor::Create(m_pGraphicDev);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;
	
	vPos = { 0.f, 0.f, 0.f };
	vScale = { 250.f * fMultiply, 250.f * fMultiply, 1.f };

// 	vPos.x = vPos.x - WINCX * 0.5f;
// 	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

void CMainGame_Arrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();

	//vPos = { 400.f, 50.f, 0.f };
	//vScale = { 359.f * fMultiply, 81.f * fMultiply, 1.f };
	//
	//vPos.x = vPos.x - WINCX * 0.5f;
	//vPos.y = -vPos.y + WINCY * 0.5f;

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

_int CMainGame_Arrow::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);



	KeyInput();

	__super::Update_GameObject(fTimeDelta);

	return _int();
}

void CMainGame_Arrow::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

HRESULT CMainGame_Arrow::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGame_Arrow_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}



void CMainGame_Arrow::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CEventMgr::GetInstance()->OffMiniGame_Arrow(SCENETAG::LOBBY);
	}



}





CMainGame_Arrow* CMainGame_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainGame_Arrow* pInstance = new CMainGame_Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyDialog Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMainGame_Arrow::Free()
{
	__super::Free();
}
