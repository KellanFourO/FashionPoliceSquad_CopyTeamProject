#include "stdafx.h"
#include <random>

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
	Free();
}



HRESULT CMainGame_Arrow::Ready_GameObject()
{
	//////////////랜덤값 구하기////////////////////////
	// 시드 값으로 사용할 난수 엔진 생성
	random_device rd;

	// 시드 값을 사용하여 유사 난수 엔진 생성
	mt19937 gen(rd());

	// 정수 분포 (예: 0 이상 3 이하의 난수)
	uniform_int_distribution<int> distribution(0, 3);

	// 랜덤 숫자 생성 방법
	// 	int random_number = distribution(gen);
	///////////////////////////////////////////////////

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//State Icon 만들기
	m_pStateIcon = CMini_StateIcon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pStateIcon, E_FAIL);

	//Arrow 만들기
	for (int i = 0; i != m_ArrowCount; ++i)
	{
		int random_number = distribution(gen);
		int iTempIndex = m_ArrowCount - (m_ArrowCount - i);

		CMini_Arrow* pArrow = CMini_Arrow::Create(m_pGraphicDev, iTempIndex, random_number);
		NULL_CHECK_RETURN(pArrow, E_FAIL);

		m_pVecArrow.push_back(pArrow);
	}
	m_pCopyVector = m_pVecArrow;

	//Arrow 위치 바탕으로 Cursor 만들기
	_vec3 m_vCursorPos = m_pVecArrow.back()->Get_ArrowPos();

	m_pCursor = CMini_Cursor::Create(m_pGraphicDev, m_vCursorPos);
	NULL_CHECK_RETURN(m_pCursor, E_FAIL);

	//TimeBar 만들기(1)배경, (2)심지
	m_pTimeBar2 = CMini_TimeBar::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(m_pTimeBar2, E_FAIL);
	m_pTimeBar = CMini_TimeBar::Create(m_pGraphicDev, 0);
	NULL_CHECK_RETURN(m_pTimeBar, E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 400.f, 300.f, 0.f };
	vScale = { 250.f * fMultiply, 250.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	m_eGameState = CMainGame_Arrow::ArrowGameState::ING;

	return S_OK;
}

void CMainGame_Arrow::Render_GameObject()
{
	if (!m_ClearCheck && !m_LoseCheck) {

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



		m_pStateIcon->Render_GameObject();

		for (auto& iter : m_pVecArrow)
		{
			iter->Render_GameObject();
		}

		m_pCursor->Render_GameObject();
		m_pTimeBar2->Render_GameObject();
		m_pTimeBar->Render_GameObject();
	}
}

_int CMainGame_Arrow::Update_GameObject(const _float& fTimeDelta)
{
	m_ClearCheck = CEventMgr::GetInstance()->Get_MiniGameClearCheck(0);
	m_LoseCheck = CEventMgr::GetInstance()->Get_MiniGameLoseCheck(0);

	if (!m_ClearCheck && !m_LoseCheck) {

		Engine::Add_RenderGroup(RENDER_UI, this);

		GameState_Update();

		KeyInput();
		State_Icon_Update();

		__super::Update_GameObject(fTimeDelta);


		m_pStateIcon->Update_GameObject(fTimeDelta);

		for (auto& iter : m_pVecArrow)
		{
			iter->Update_GameObject(fTimeDelta);
		}

		m_pCursor->Update_GameObject(fTimeDelta);
		m_pTimeBar2->Update_GameObject(fTimeDelta);
		m_pTimeBar->Update_GameObject(fTimeDelta);
	}

// 	if (m_eGameState == CMainGame_Arrow::ArrowGameState::LOSE)
// 	{
// 		return OBJ_DEAD;
// 	}

	return _int();
}

void CMainGame_Arrow::LateUpdate_GameObject()
{
	if (!m_ClearCheck && !m_LoseCheck) {
		CGameObject::LateUpdate_GameObject();

		m_pStateIcon->LateUpdate_GameObject();

		for (auto& iter : m_pVecArrow)
		{
			iter->LateUpdate_GameObject();
		}

		m_pCursor->LateUpdate_GameObject();
		m_pTimeBar2->LateUpdate_GameObject();
		m_pTimeBar->LateUpdate_GameObject();

		if (m_pTimeBar2->Get_TimeOverCheck() == true)
		{
			m_eGameState = CMainGame_Arrow::ArrowGameState::LOSE;
		}
	}
}

void CMainGame_Arrow::Reset()
{
	m_eGameState = CMainGame_Arrow::ArrowGameState::State_END;
	m_ClearCheck = false;
	m_LoseCheck = false;

	// 시드 값으로 사용할 난수 엔진 생성
	random_device rd;

	// 시드 값을 사용하여 유사 난수 엔진 생성
	mt19937 gen(rd());

	// 정수 분포 (예: 0 이상 3 이하의 난수)
	uniform_int_distribution<int> distribution(0, 3);

	m_pTimeBar2->Set_TimerReset();

	Safe_Release(m_pCursor);

	if (!m_pVecArrow.empty()) {
		for (auto& iter : m_pVecArrow)
		{
			Safe_Release(iter);
		}
		m_pVecArrow.clear();
	}
	if (!m_pCopyVector.empty()) {
		for (auto& iter : m_pCopyVector)
		{
			Safe_Release(iter);
		}
		m_pCopyVector.clear();
	}

	for (int i = 0; i != m_ArrowCount; ++i)
	{
		int random_number = distribution(gen);
		int iTempIndex = m_ArrowCount - (m_ArrowCount - i);

		CMini_Arrow* pArrow = CMini_Arrow::Create(m_pGraphicDev, iTempIndex, random_number);

		m_pVecArrow.push_back(pArrow);
	}
	m_pCopyVector = m_pVecArrow;

	_vec3 m_vCursorPos = m_pVecArrow.back()->Get_ArrowPos();
	m_pCursor = CMini_Cursor::Create(m_pGraphicDev, m_vCursorPos);
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MainGame_Arrow_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}

HRESULT CMainGame_Arrow::State_Icon_Update()
{
	if (m_NOP_Mode == true)
	{
		m_pStateIcon->Change_State(STATEICON_NOP);
		m_iCountNop++;
	}
	else if (m_NOP_Mode == false)
	{
		if (m_pVecArrow.size() >= 20)
			m_pStateIcon->Change_State(STATEICON_NORMAL);

		else if ((m_pVecArrow.size() >= 10) && (m_pVecArrow.size() < 20))
			m_pStateIcon->Change_State(STATEICON_GOOD);

		else if ((m_pVecArrow.size() >= 0) && (m_pVecArrow.size() < 10))
			m_pStateIcon->Change_State(STATEICON_PERPECT);
	}
	if (m_iCountNop > 20) {
		m_NOP_Mode = false;
		m_iCountNop = 0;
	}
	return S_OK;
}

HRESULT CMainGame_Arrow::GameState_Update()
{
	if (m_eGameState == CMainGame_Arrow::ArrowGameState::CLEAR)
	{
		CEventMgr::GetInstance()->Call_MiniGameResult(3, 0);
		CEventMgr::GetInstance()->OffMiniGame_Arrow(SCENETAG::LOBBY, true);
	}

	if (m_eGameState == CMainGame_Arrow::ArrowGameState::LOSE)
	{
		CEventMgr::GetInstance()->Call_MiniGameResult(3, 1);
		CEventMgr::GetInstance()->OffMiniGame_Arrow(SCENETAG::LOBBY, false);
	}
	return S_OK;
}

void CMainGame_Arrow::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CEventMgr::GetInstance()->OffMiniGame_Arrow(SCENETAG::LOBBY, true);
	}

	if ((Engine::Get_DIKeyState(DIK_UP) & 0x80) ||
		(Engine::Get_DIKeyState(DIK_DOWN) & 0x80) ||
		(Engine::Get_DIKeyState(DIK_LEFT) & 0x80) ||
		(Engine::Get_DIKeyState(DIK_RIGHT) & 0x80))
	{
		if (Engine::Get_DIKeyState(DIK_UP) & 0x80 && !m_bUpPressed)
		{	m_myKeyType = MINIGAME_ARROW_TYPE::UP;
			m_bUpPressed = true;
			if (m_pVecArrow.back()->Get_AroowType() != m_myKeyType)
			{
				m_NOP_Mode = true;
			}
		}
		if (Engine::Get_DIKeyState(DIK_DOWN) & 0x80 && !m_bDownPressed)
		{	m_myKeyType = MINIGAME_ARROW_TYPE::DOWN;
			m_bDownPressed = true;
			if (m_pVecArrow.back()->Get_AroowType() != m_myKeyType)
			{
				m_NOP_Mode = true;
			}
		}
		if (Engine::Get_DIKeyState(DIK_LEFT) & 0x80 && !m_bLeftPressed)
		{	m_myKeyType = MINIGAME_ARROW_TYPE::LEFT;
			m_bLeftPressed = true;
			if (m_pVecArrow.back()->Get_AroowType() != m_myKeyType)
			{
				m_NOP_Mode = true;
			}
		}
		if (Engine::Get_DIKeyState(DIK_RIGHT) & 0x80 && !m_bRightPressed)
		{	m_myKeyType = MINIGAME_ARROW_TYPE::RIGHT;
			m_bRightPressed = true;
			if (m_pVecArrow.back()->Get_AroowType() != m_myKeyType)
			{
				m_NOP_Mode = true;
			}
		}

		else if (m_pVecArrow.back()->Get_AroowType() == m_myKeyType)
		{
			if (m_pVecArrow.size() <= 1)
			{
				m_eGameState = CMainGame_Arrow::ArrowGameState::CLEAR;
			}
			else {
				// CMini_Arrow* lastElementPtr = m_pVecArrow.back();
				// delete lastElementPtr;
				m_NOP_Mode = false;
				m_pVecArrow.pop_back();
				_vec3 PosTemp = m_pVecArrow.back()->Get_ArrowPos();
				m_pCursor->Set_Pos_ArrowPoint(PosTemp);
				m_myKeyType = MINIGAME_ARROW_TYPE::MINIGAME_ARROW_END;
			}
		}

	}

	if (!(Engine::Get_DIKeyState(DIK_UP) & 0x80))
	{
		m_bUpPressed = false;
	}
	if (!(Engine::Get_DIKeyState(DIK_DOWN) & 0x80))
	{
		m_bDownPressed = false;
	}
	if (!(Engine::Get_DIKeyState(DIK_LEFT) & 0x80))
	{
		m_bLeftPressed = false;
	}
	if (!(Engine::Get_DIKeyState(DIK_RIGHT) & 0x80))
	{
		m_bRightPressed = false;
	}

}


CMainGame_Arrow* CMainGame_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainGame_Arrow* pInstance = new CMainGame_Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MiniGame_Arrow Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMainGame_Arrow::Free()
{

	if (!m_pCopyVector.empty())
	{
		for (int i = 0; i != m_pCopyVector.size(); )
		{
			Safe_Release(m_pCopyVector[i]);
		}
		m_pCopyVector.clear();
	}

	Safe_Release(m_pCursor);
	Safe_Release(m_pStateIcon);
	Safe_Release(m_pTimeBar);
	Safe_Release(m_pTimeBar2);

	__super::Free();
}
