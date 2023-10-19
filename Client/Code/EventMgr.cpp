#include "EventMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include <random>
#include "LoadingStage1.h"
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

void CEventMgr::OnCard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, DIALOGTAG eDialogTag)
{

	Engine::CCardList* pCardList = nullptr;
	pCardList = Engine::CCardList::Create(pGraphicDev, eDialogTag);

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

	m_pDialog = nullptr;
	m_pDialog = CMyDialog::Create(pGraphicDev, eDialogTag);

	NULL_CHECK(m_pDialog);

	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::DIALOG, m_pDialog);

}

void CEventMgr::OnPause(_bool bPause, SCENETAG eSceneTag)
{
	if (Management()->Get_Scene()->Get_SceneTag() == eSceneTag)
	{
		Management()->Get_Scene()->Set_Pause(bPause);
	}
}

void CEventMgr::OnDropItem(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, _int iCount)
{
	CItem* pItem = nullptr;
	CLayer* pLayer = Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::GAMELOGIC);
	CPlayer* pPlayer = Management()->Get_Player();

	_vec3 vPlayerPos = pPlayer->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vPlayerRight = pPlayer->Get_Transform()->m_vInfo[INFO_RIGHT];
	_vec3 vPlayerUp = pPlayer->Get_Transform()->m_vInfo[INFO_UP];
	_vec3 vPlayerLook = pPlayer->Get_Transform()->m_vInfo[INFO_LOOK];

	for (int i = 0; i < iCount; ++i)
	{
		random_device rd;
		mt19937 gen(rd());

		uniform_int_distribution<int> distributionID(0, 4);


		pItem = CItem::Create(pGraphicDev);
		pLayer->Add_GameObject(OBJECTTAG::ITEM,pItem);

		_vec3 vRandomPos;

		vRandomPos.x = vPlayerPos.x + static_cast<float>(rand()) / RAND_MAX * 100 * 2 - 100;
		vRandomPos.y = vPlayerPos.y;
		vRandomPos.z = vPlayerPos.z + static_cast<float>(rand()) / RAND_MAX * 100 * 2 - 100;

		Item_Info tInfo;
		tInfo.Item_ID = static_cast<ItemID>(distributionID(gen));
		tInfo.vPos = vRandomPos;

		pItem->Set_INFO(tInfo);
	}

}

void CEventMgr::OffDialog()
{
	m_pDialog->Set_EndInput(true);
}

void CEventMgr::SceneChange(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
		CLoadingStage1* pScene = nullptr;

		pScene = CLoadingStage1::Create(pGraphicDev, eSceneTag);

		//CUIMgr::GetInstance()->DestroyInstance();
		Management()->Get_Player()->Set_SceneChange(true);
		Management()->Set_SYSceneChange(true);
		Management()->Change_Scene(pScene);
}

HRESULT CEventMgr::OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	if (pGame_Arrow != nullptr)
	{
		Safe_Release(pGame_Arrow);
// 		delete pGame_Arrow;
// 		pGame_Arrow = nullptr;
	}

	m_eMiniGameState = CEventMgr::MiniGameState::PLAY_NOW;


	pGame_Arrow = Engine::CMainGame_Arrow::Create(pGraphicDev);
	NULL_CHECK_RETURN(pGame_Arrow, E_FAIL);

	SoundMgr()->StopSound(SOUND_BGM);
	OnPause(TRUE, SCENETAG::LOBBY);
	SoundMgr()->PlayBGM(L"MiniGame1BGM.mp3",1.f);

	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Add_GameObject(OBJECTTAG::MINIGAME, pGame_Arrow);

	return S_OK;
}

HRESULT CEventMgr::OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	if (pGame_KickBoard != nullptr)
	{
		Safe_Release(pGame_KickBoard);
// 		delete pGame_KickBoard;
// 		pGame_KickBoard = nullptr;
	}
	
	m_eMiniGameState = CEventMgr::MiniGameState::PLAY_NOW;

	pGame_KickBoard = Engine::CMainGame_KickBoard::Create(pGraphicDev);
	NULL_CHECK_RETURN(pGame_KickBoard, E_FAIL);

	OnPause(TRUE, SCENETAG::LOBBY);

	SoundMgr()->StopSound(SOUND_BGM);
	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Add_GameObject(OBJECTTAG::MINIGAME, pGame_KickBoard);
	SoundMgr()->PlayBGM(L"MiniGame2BGM.mp3", 1.f);

	return S_OK;
}

HRESULT CEventMgr::OnMiniGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	m_bMiniGame_ClearCheck[2] = true;

	SoundMgr()->StopSound(SOUND_BGM);
	OnPause(FALSE, SCENETAG::LOBBY);
	SoundMgr()->PlayBGM(L"QuizBGM.mp3", 1.f);
// 	m_eMiniGameState = CEventMgr::MiniGameState::PLAY_NOW;
// 	pGame_Quiz = Engine::CMainGame_Quiz::Create(pGraphicDev);
// 	NULL_CHECK_RETURN(pGame_Quiz, E_FAIL);
//
// 	OnPause(TRUE, SCENETAG::LOBBY);
//
// 	Management()->Get_One_Scene(eSceneTag)->Get_Layer(LAYERTAG::MINIGAME)->Add_GameObject(OBJECTTAG::MINIGAME, pGame_KickBoard);

	return S_OK;
}


HRESULT CEventMgr::OffMiniGame_Arrow(SCENETAG eSceneTag, _bool ClearCheck)
{
	m_eMiniGameState = CEventMgr::MiniGameState::NOT_PLAY;
	if (ClearCheck == true)
	{
		Set_MiniGameClearCheck(0, TRUE);
		SoundMgr()->StopSound(SOUND_BGM);
		OnPause(FALSE, SCENETAG::LOBBY);
		SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 0.25f);
	}
	if (ClearCheck == false)
	{
		Set_MiniGameClearCheck(0, FALSE);
		OnPause(FALSE, SCENETAG::LOBBY);
	}

// 	if (pGame_Arrow)
// 	{
// 		Safe_Release(pGame_Arrow);
// // 		delete pGame_Arrow;
// // 		pGame_Arrow = nullptr;
// 	}

	SoundMgr()->StopSound(SOUND_BGM);
	SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 1);

	return S_OK;
}
HRESULT CEventMgr::OffMiniGame_KickBoard(SCENETAG eSceneTag, _bool ClearCheck)
{
	m_eMiniGameState = CEventMgr::MiniGameState::NOT_PLAY;
	if (ClearCheck == true)
	{
		Set_MiniGameClearCheck(1, TRUE);
		SoundMgr()->StopSound(SOUND_BGM);
		OnPause(FALSE, SCENETAG::LOBBY);
		SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 0.25f);
	}
	if (ClearCheck == false)
	{
		Set_MiniGameClearCheck(1, FALSE);
		OnPause(FALSE, SCENETAG::LOBBY);
	}
	
// 	if (pGame_KickBoard)
// 	{
// 		Safe_Release(pGame_KickBoard);
// // 		delete pGame_KickBoard;
// // 		pGame_KickBoard = nullptr;
// 	}

	SoundMgr()->StopSound(SOUND_BGM);
	SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 1);

	return S_OK;
}
HRESULT CEventMgr::OffMiniGame_Quiz(SCENETAG eSceneTag, _bool ClearCheck)
{

	SoundMgr()->StopSound(SOUND_BGM);
	OnPause(FALSE, SCENETAG::LOBBY);
	SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 0.25f);
	return S_OK;
}


void CEventMgr::Free()
{
	if (pGame_Arrow != nullptr)
	Safe_Release(pGame_Arrow);

	if (pGame_KickBoard != nullptr)
	Safe_Release(pGame_KickBoard);
}
