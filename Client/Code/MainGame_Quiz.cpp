
#include "stdafx.h"
#include "MainGame_Quiz.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "EventMgr.h"

CMainGame_Quiz::CMainGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMainGame_Quiz::CMainGame_Quiz(const CMainGame_Quiz& rhs)
	: Engine::CGameObject(rhs)
{
}

CMainGame_Quiz::~CMainGame_Quiz()
{


}


HRESULT CMainGame_Quiz::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_eGameState = CMainGame_KickBoard::KickBoard_GameState::ING;

	//소속객체는 여기서 만들기

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 400.f, 300.f, 0.f };
	vScale = { 250.f * fMultiply, 250.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

}

void CMainGame_Quiz::Render_GameObject()
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
	}
}

_int CMainGame_Quiz::Update_GameObject(const _float& fTimeDelta)
{
	m_ClearCheck = CEventMgr::GetInstance()->Get_MiniGameClearCheck(2);

	if (!m_ClearCheck) {

		Engine::Add_RenderGroup(RENDER_UI, this);

		GameState_Update();

		KeyInput();

		__super::Update_GameObject(fTimeDelta);



	}

	return 0;
}

void CMainGame_Quiz::LateUpdate_GameObject()
{
	if (!m_ClearCheck) {
		CGameObject::LateUpdate_GameObject();
	}
}

HRESULT CMainGame_Quiz::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MainGame_Arrow_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMainGame_Quiz::KeyInput()
{
}

HRESULT CMainGame_Quiz::GameState_Update()
{
	if (m_eGameState == CMainGame_Quiz::QuizGameState::CLEAR)
	{
		MSG_BOX("Clear!");
		CEventMgr::GetInstance()->OffMiniGame_KickBoard(SCENETAG::LOBBY, true);
	}

	if (m_eGameState == CMainGame_Quiz::QuizGameState::LOSE)
	{
		MSG_BOX("Lose...");
		CEventMgr::GetInstance()->OffMiniGame_KickBoard(SCENETAG::LOBBY, false);
	}
	return S_OK;
}


CMainGame_Quiz* CMainGame_Quiz::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainGame_Quiz* pInstance = new CMainGame_Quiz(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MiniGame_Arrow Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMainGame_Quiz::Free()
{
	// 
	// 	if (!m_pCopyVector.empty())
	// 	{
	// 		for (int i = 0; i != m_pCopyVector.size(); )
	// 		{
	// 			Safe_Release(m_pCopyVector[i]);
	// 		}
	// 		m_pCopyVector.clear();
	// 	}
	// 
	// 	Safe_Release(m_pCursor);
	// 	Safe_Release(m_pStateIcon);
	// 	Safe_Release(m_pTimeBar);
	// 	Safe_Release(m_pTimeBar2);

	__super::Free();

}
