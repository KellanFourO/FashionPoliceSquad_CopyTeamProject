#include "EventMgr.h"
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

void CEventMgr::OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	vector<CGameObject*> TempVector = Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Get_ObjectList(OBJECTTAG::MINIGAME);
	TempVector.clear();

	pGame_Arrow = Engine::CMainGame_Arrow::Create(pGraphicDev);

	OnPause(TRUE, SCENETAG::LOBBY);
	
	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Add_GameObject(OBJECTTAG::MINIGAME, pGame_Arrow);
}

void CEventMgr::OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	m_bMiniGame_ClearCheck[1] = true;
	//OnPause(FALSE, SCENETAG::LOBBY);
}

void CEventMgr::OnMiniGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	m_bMiniGame_ClearCheck[2] = true;
	//OnPause(FALSE, SCENETAG::LOBBY);
}




void CEventMgr::OffMiniGame_Arrow(SCENETAG eSceneTag)
{
	Set_MiniGameClearCheck(0, TRUE);
	OnPause(FALSE, SCENETAG::LOBBY);
	
	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Delete_GameObject(OBJECTTAG::MINIGAME, pGame_Arrow, pGame_Arrow->m_iIndex);
}
void CEventMgr::OffMiniGame_KickBoard(SCENETAG eSceneTag)
{

}
void CEventMgr::OffMiniGame_Quiz(SCENETAG eSceneTag)
{

}




void CEventMgr::Free()
{

}
