#include "stdafx.h"
#include <random>

#include "MainGame_KickBoard.h"
#include "EventMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"

CMainGame_KickBoard::CMainGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMainGame_KickBoard::CMainGame_KickBoard(const CMainGame_KickBoard& rhs)
	: CGameObject(rhs)
{
}

CMainGame_KickBoard::~CMainGame_KickBoard()
{
}

HRESULT CMainGame_KickBoard::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//TimeBar ¸¸µé±â
// 	m_pTimeBar2 = CMini_TimeBar::Create(m_pGraphicDev, 1);
// 	NULL_CHECK_RETURN(m_pTimeBar2, E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 400.f, 300.f, 0.f };
	vScale = { 250.f * fMultiply, 250.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);
	return S_OK;
}

void CMainGame_KickBoard::Render_GameObject()
{
	if (!m_ClearCheck) {

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

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

// 		m_pStateIcon->Render_GameObject();
// 
	}
}

_int CMainGame_KickBoard::Update_GameObject(const _float& fTimeDelta)
{
	m_ClearCheck = CEventMgr::GetInstance()->Get_MiniGameClearCheck(1);

	if (!m_ClearCheck) {

		Engine::Add_RenderGroup(RENDER_UI, this);

 		GameState_Update();
		KeyInput();

 		__super::Update_GameObject(fTimeDelta);

// 		m_pStateIcon->Update_GameObject(fTimeDelta);
// 
	}

	return _int();
}

void CMainGame_KickBoard::LateUpdate_GameObject()
{
	if (!m_ClearCheck) {
		CGameObject::LateUpdate_GameObject();

// 		m_pStateIcon->LateUpdate_GameObject();
// 
// 		if (m_pTimeBar2->Get_TimeOverCheck() == true)
// 		{
// 			m_eGameState = CMainGame_KickBoard::ArrowGameState::LOSE;
// 		}
	}
}

HRESULT CMainGame_KickBoard::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MainGame_KickBoard_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

HRESULT CMainGame_KickBoard::GameState_Update()
{
	if (m_eGameState == CMainGame_KickBoard::KickBoard_GameState::CLEAR)
	{
		MSG_BOX("Clear!");
		CEventMgr::GetInstance()->OffMiniGame_KickBoard(SCENETAG::LOBBY, true);
		//Safe_Release(*this);
	}

	if (m_eGameState == CMainGame_KickBoard::KickBoard_GameState::LOSE)
	{
		MSG_BOX("Lose...");
		CEventMgr::GetInstance()->OffMiniGame_KickBoard(SCENETAG::LOBBY, false);
		//Safe_Release(*this);
	}
	return S_OK;
}

void CMainGame_KickBoard::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CEventMgr::GetInstance()->OffMiniGame_Arrow(SCENETAG::LOBBY, true);
	}


}

CMainGame_KickBoard* CMainGame_KickBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainGame_KickBoard* pInstance = new CMainGame_KickBoard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MiniGame_KickBoard Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMainGame_KickBoard::Free()
{
// 	if (!m_pCopyVector.empty())
// 	{
// 		for (int i = 0; i != m_pCopyVector.size(); )
// 		{
// 			Safe_Release(m_pCopyVector[i]);
// 		}
// 	}




	__super::Free();



}
