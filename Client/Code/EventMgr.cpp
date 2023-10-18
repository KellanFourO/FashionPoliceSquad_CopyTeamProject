#include "EventMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CEventMgr)

CEventMgr::CEventMgr()
{
}


CEventMgr::~CEventMgr()
{
	Free();
}

void CEventMgr::GetEventColider()
{
}

void CEventMgr::OnLevelUp(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	Engine::CCardList* pCardList = nullptr;
	pCardList = Engine::CCardList::Create(pGraphicDev);

	NULL_CHECK(pCardList);

	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::CARDLIST, pCardList);

}

void CEventMgr::OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, DIALOGTAG eDialogTag)
{
	//pGameObject = CMyDialog::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::DIALOG, pGameObject), E_FAIL);
	//
	//pGameObject = CPortrait::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PORTRAIT, pGameObject), E_FAIL);

	CMyDialog* pDialog = nullptr;
	pDialog = CMyDialog::Create(pGraphicDev, eDialogTag);

	NULL_CHECK(pDialog);

	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::DIALOG, pDialog);

}

void CEventMgr::OnPause(_bool bPause, SCENETAG eSceneTag)
{
	if (Management()->Get_Scene()->Get_SceneTag() == eSceneTag)
	{
		Management()->Get_Scene()->Set_Pause(bPause);
	}
}

HRESULT CEventMgr::OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	m_eMiniGameState = CEventMgr::MiniGameState::PLAY_NOW;
	vector<CGameObject*> TempVector = Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Get_ObjectList(OBJECTTAG::MINIGAME);
	TempVector.clear();

	pGame_Arrow = Engine::CMainGame_Arrow::Create(pGraphicDev);
	NULL_CHECK_RETURN(pGame_Arrow, E_FAIL);

	OnPause(TRUE, SCENETAG::LOBBY);
	
	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Add_GameObject(OBJECTTAG::MINIGAME, pGame_Arrow);
	
	return S_OK;
}

HRESULT CEventMgr::OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	m_eMiniGameState = CEventMgr::MiniGameState::PLAY_NOW;
	pGame_KickBoard = Engine::CMainGame_KickBoard::Create(pGraphicDev);
	NULL_CHECK_RETURN(pGame_KickBoard, E_FAIL);

	OnPause(TRUE, SCENETAG::LOBBY);

	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Add_GameObject(OBJECTTAG::MINIGAME, pGame_KickBoard);

	return S_OK;
}

HRESULT CEventMgr::OnMiniGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	m_bMiniGame_ClearCheck[2] = true;
	//OnPause(FALSE, SCENETAG::LOBBY);
	return S_OK;
}


HRESULT CEventMgr::OffMiniGame_Arrow(SCENETAG eSceneTag, _bool ClearCheck)
{
	m_eMiniGameState = CEventMgr::MiniGameState::NOT_PLAY;
	if (ClearCheck == true)
	{
		Set_MiniGameClearCheck(0, TRUE);
		OnPause(FALSE, SCENETAG::LOBBY);
	}
	if (ClearCheck == false)
	{
		Set_MiniGameClearCheck(0, FALSE);
		OnPause(FALSE, SCENETAG::LOBBY);
	} 
	return S_OK;
}
HRESULT CEventMgr::OffMiniGame_KickBoard(SCENETAG eSceneTag, _bool ClearCheck)
{
	m_eMiniGameState = CEventMgr::MiniGameState::NOT_PLAY;
	if (ClearCheck == true)
	{
		Set_MiniGameClearCheck(1, TRUE);
		OnPause(FALSE, SCENETAG::LOBBY);
	}
	if (ClearCheck == false)
	{
		Set_MiniGameClearCheck(1, FALSE);
		OnPause(FALSE, SCENETAG::LOBBY);
	}
	return S_OK;
}
HRESULT CEventMgr::OffMiniGame_Quiz(SCENETAG eSceneTag, _bool ClearCheck)
{
	return S_OK;
}


void CEventMgr::Free()
{
	Safe_Release(pGame_Arrow);
	Safe_Release(pGame_KickBoard);
}
