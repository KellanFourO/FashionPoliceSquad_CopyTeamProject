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

	m_eGameState = CMainGame_KickBoard::KickBoard_GameState::ING;
	

	//Player 만들기
	m_pPlayer = CMini_Player::Create(m_pGraphicDev);
 	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 400.f, 300.f, 0.f };
	vScale = { 300.f * fMultiply, 200.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_defRect.Up_Y = vPos.y + (vScale.y * 0.9f);
	m_defRect.Down_Y = vPos.y - (vScale.y * 0.9f);
	m_defRect.Left_X = vPos.x - (vScale.x * 0.9f);
	m_defRect.Right_X = vPos.x + (vScale.x * 0.9f);

	RectSizeX = fabs(m_defRect.Right_X - m_defRect.Left_X);
	STDPointX = RectSizeX / 20;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	m_pPlayer->Set_Rect(m_defRect);

	//Timer 만들기
	for (int i = 0; i != m_Timer_Score_Num; ++i)
	{
		float fsize = 15.f + 5.f;
		m_pTimer[i] = CMini_Timer::Create(m_pGraphicDev, 150.f + (i * fsize), i);
		NULL_CHECK_RETURN(m_pTimer[i], E_FAIL);
	}
	//Score 만들기
	for (int i = 0; i != m_Timer_Score_Num; ++i)
	{
		float fsize = 15.f + 5.f;
		m_pScore[i] = CMini_Score::Create(m_pGraphicDev, 600.f + (i * fsize), i);
		NULL_CHECK_RETURN(m_pScore[i], E_FAIL);
	}

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


		if (!m_vecGold.empty()) {
			for (auto& iter : m_vecGold)
			{
				iter->Render_GameObject();
			}
		}

		if (!m_vecEnemy.empty()) {
			for (auto& iter : m_vecEnemy)
			{
				iter->Render_GameObject();
			}
		}
 
		for (int i = 0; i != m_Timer_Score_Num; ++i)
		{
			m_pTimer[i]->Render_GameObject();
		}

		for (int i = 0; i != m_Timer_Score_Num; ++i)
		{
			m_pScore[i]->Render_GameObject();
		}

 		m_pPlayer->Render_GameObject();
	}
}

_int CMainGame_KickBoard::Update_GameObject(const _float& fTimeDelta)
{
	m_ClearCheck = CEventMgr::GetInstance()->Get_MiniGameClearCheck(1);

	if (!m_ClearCheck) {

		Engine::Add_RenderGroup(RENDER_UI, this);

		TimeCheck();
		CoinCount();

		Collisoin_Check();
		GameState_Update();

		KeyInput();


 		__super::Update_GameObject(fTimeDelta);

		if (!m_vecGold.empty()) {
			for (auto& iter : m_vecGold)
			{
				iter->Update_GameObject(fTimeDelta);
			}
		}

		if (!m_vecEnemy.empty()) {
			for (auto& iter : m_vecEnemy)
			{
				iter->Update_GameObject(fTimeDelta);
			}
		}

		for (int i = 0; i != m_Timer_Score_Num; ++i)
		{
			m_pTimer[i]->Update_GameObject(fTimeDelta);
		}

		for (int i = 0; i != m_Timer_Score_Num; ++i)
		{
			m_pScore[i]->Update_GameObject(fTimeDelta);
		}

 		m_pPlayer->Update_GameObject(fTimeDelta);

		//ENEMY 생성 (한 번에 둘!)
		if (m_EnemyCreatCount % 400 == 0) {
			random_device rd;
			mt19937 gen(rd());

			// 정수 분포 (예: 0 이상 3 이하의 난수)
			uniform_int_distribution<int> distribution(1, 10); //ENEMY 생성위치
			uniform_int_distribution<int> distribution2(0, 8); //ENEMY 텍스쳐

			int r_Point = distribution(gen);
			int r_Point2 = distribution(gen);
			while(r_Point == r_Point2)
			{
				r_Point2 = distribution(gen);
			}

			int r_TexType = distribution2(gen);
			int r_TexType2 = distribution2(gen);
			while (r_TexType == r_TexType2)
			{
				r_TexType2 = distribution2(gen);
			}

			CMini_Enemy* m_pEnemy = CMini_Enemy::Create(
				m_pGraphicDev, (m_defRect.Left_X + ((STDPointX * 2) * r_Point)), r_TexType);
			NULL_CHECK_RETURN(m_pEnemy, E_FAIL);

			m_vecEnemy.push_back(m_pEnemy);

			CMini_Enemy* m_pEnemy2 = CMini_Enemy::Create(
				m_pGraphicDev, (m_defRect.Left_X + ((STDPointX * 2) * r_Point2)), r_TexType2);
			NULL_CHECK_RETURN(m_pEnemy2, E_FAIL);

			m_vecEnemy.push_back(m_pEnemy2);

			m_EnemyCreatCount = 0;
		}
		m_EnemyCreatCount++;

		//Gold 생성
		if (m_GoldCreatCount % 300 == 0) {
			random_device rd;
			mt19937 gen(rd());

			// 정수 분포 (예: 0 이상 3 이하의 난수)
			uniform_int_distribution<int> distribution(1, 20); //ENEMY 생성위치

			int r_Point = distribution(gen);

			CMini_Gold* m_pGold = CMini_Gold::Create(
				m_pGraphicDev, (m_defRect.Left_X + (STDPointX * r_Point)));
			NULL_CHECK_RETURN(m_pGold, E_FAIL);

			m_vecGold.push_back(m_pGold);

			m_GoldCreatCount = 0;
		}
		m_GoldCreatCount++;
	}

	return 0;
}

void CMainGame_KickBoard::LateUpdate_GameObject()
{
	if (!m_ClearCheck) {
		CGameObject::LateUpdate_GameObject();
 
		if (!m_vecGold.empty()) {
			for (auto& iter : m_vecGold)
			{
				iter->LateUpdate_GameObject();
			}
		}

		if (!m_vecEnemy.empty()) {
			for (auto& iter : m_vecEnemy)
			{
				iter->LateUpdate_GameObject();
			}
		}

		for (int i = 0; i != m_Timer_Score_Num; ++i)
		{
			m_pTimer[i]->LateUpdate_GameObject();
		}

		for (int i = 0; i != m_Timer_Score_Num; ++i)
		{
			m_pScore[i]->LateUpdate_GameObject();
		}

		m_pPlayer->LateUpdate_GameObject();

		
		//if (m_pTimeBar2->Get_TimeOverCheck() == true)
 		//{
 		//	m_eGameState = CMainGame_KickBoard::ArrowGameState::LOSE;
 		//}
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
	}

	if (m_eGameState == CMainGame_KickBoard::KickBoard_GameState::LOSE)
	{
		MSG_BOX("Lose...");
		CEventMgr::GetInstance()->OffMiniGame_KickBoard(SCENETAG::LOBBY, false);
	}
	return S_OK;
}

void CMainGame_KickBoard::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CEventMgr::GetInstance()->OffMiniGame_KickBoard(SCENETAG::LOBBY, true);
	}

 	if (Engine::Get_DIMouseState(DIM_LB) & 0x80  &&  m_bMousePressed == false)
 	{
 		m_pPlayer->Set_Dir();
		m_bMousePressed = true;
 	}
	if (!(Engine::Get_DIMouseState(DIM_LB) & 0x80))
	{
		m_bMousePressed = false;
	}
}

void CMainGame_KickBoard::TimeCheck()
{
	if (m_RealTime > 0) {
		--m_TimeFrame;

		if (m_TimeFrame == 0)
		{
			--m_RealTime;
			m_TimeFrame = 60;
		}
	}
	_int	Ten_Num = m_RealTime / 10;
	_int	One_Num = m_RealTime % 10;

	m_pTimer[1]->Set_TexNum(Ten_Num);
	m_pTimer[2]->Set_TexNum(One_Num);

	if ((m_RealTime <= 0) && m_iMyCoinCount < 10)
	{
		m_eGameState = CMainGame_KickBoard::KickBoard_GameState::LOSE;
	}
	if ((m_RealTime > 0) && m_iMyCoinCount >= 10)
	{
		m_eGameState = CMainGame_KickBoard::KickBoard_GameState::CLEAR;
	}
}

void CMainGame_KickBoard::CoinCount()
{

	_int	Ten_Num2 = m_iMyCoinCount / 10;
	_int	One_Num2 = m_iMyCoinCount % 10;

	m_pScore[1]->Set_TexNum(Ten_Num2);
	m_pScore[2]->Set_TexNum(Ten_Num2);
}

void CMainGame_KickBoard::Collisoin_Check()
{
	_vec3 PlayerPos = m_pPlayer->Get_Pos();
	_float PlayerSize = m_pPlayer->Get_Size();


	//Player-Enemy
	for (auto& iter : m_vecEnemy)
	{
		if (fabs(iter->Get_Pos().x - PlayerPos.x) < ((iter->Get_Size() * 0.5f) + (PlayerSize * 0.5f))
			&& fabs(iter->Get_Pos().y - PlayerPos.y) < ((iter->Get_Size() * 0.5f) + (PlayerSize * 0.5f))\
			&& iter->Get_Dead() != CMini_Enemy::EnemyState::Dead)
		{
			m_pPlayer->Set_DamageCount();
		}
	}

	//Player-Gold
	for (auto& iter : m_vecGold)
	{
		if (fabs(iter->Get_Pos().x - PlayerPos.x) < ((iter->Get_Size() * 0.5f) + (PlayerSize * 0.5f))
			&& fabs(iter->Get_Pos().y - PlayerPos.y) < ((iter->Get_Size() * 0.5f) + (PlayerSize * 0.5f))\
			&& iter->Get_Dead() != CMini_Gold::GoldState::Dead)
		{
			m_iMyCoinCount++;
			iter->Set_Dead();
		}
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
	if (!m_vecEnemy.empty())
	{
		for (int i = 0; i != m_vecEnemy.size(); )
		{
			Safe_Release(m_vecEnemy[i]);
		}
		m_vecEnemy.clear();
	}
	if (!m_vecGold.empty())
	{
		for (int i = 0; i != m_vecGold.size(); )
		{
			Safe_Release(m_vecGold[i]);
		}
		m_vecGold.clear();
	}

	for (int i = 0; i != m_Timer_Score_Num; ++i)
	{
		Safe_Release(m_pTimer[i]);
		Safe_Release(m_pScore[i]);
	}
	Safe_Release(m_pPlayer);

	__super::Free();
}
