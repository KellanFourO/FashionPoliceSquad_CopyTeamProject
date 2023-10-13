#include "stdafx.h"
#include "LoadingStage1.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "BossStage.h"


CLoadingStage1::CLoadingStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pLoading(nullptr)
{
}

CLoadingStage1::~CLoadingStage1()
{
}

HRESULT CLoadingStage1::Ready_Scene(SCENETAG eSceneTag)
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);

	if (eSceneTag == SCENETAG::STAGE2)
	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE2);
	}
	else if (eSceneTag == SCENETAG::BOSS_STAGE)
	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_BOSS);
	}
	else if (eSceneTag == SCENETAG::LOBBY)
	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_LOBBY);
	}
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);



	return S_OK;
}

_int CLoadingStage1::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	m_pProgressBar = dynamic_cast<CProgressValue*>(Management()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::PROGRESSVALUE).back());

	if (m_pProgressBar)
	{
		m_pProgressBar->Set_Ratio(m_pLoading->Get_Value());
	}

	if (true == m_pLoading->Get_Finish())
	{
		m_pLoadingID = m_pLoading->Get_LoadingID();
		map<SCENETAG, CScene*>		m_MapSceneTemp;
		if (Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			switch (m_pLoadingID)
			{
			case CLoading::LOADING_STAGE2:
			{
				CScene* pScene = CStage2::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);

				pScene->Set_MainPlayer(m_pPlayer);

				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

				FAILED_CHECK_RETURN(Engine::COctree::GetInstance()->Ready_Octree(), E_FAIL);

				map<SCENETAG, CScene*>		m_MapSceneTemp;
				m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();
				pScene->Set_SceneTag(SCENETAG::STAGE2);
				m_MapSceneTemp.emplace(SCENETAG::STAGE2, pScene);
				Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);
				break;
			}

			case CLoading::LOADING_MAPTOOL:
			{
				CScene* pScene = CMapTool::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);


				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

				map<SCENETAG, CScene*>		m_MapSceneTemp;
				m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();

				pScene->Set_SceneTag(SCENETAG::MAPTOOL);
				m_MapSceneTemp.emplace(SCENETAG::MAPTOOL, pScene);

				Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);
				break;
			}
			case CLoading::LOADING_LOBBY:
			{
				CScene* pScene = CLobbyStage::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);


				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

				map<SCENETAG, CScene*>		m_MapSceneTemp;
				m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();

				pScene->Set_SceneTag(SCENETAG::LOBBY);
				m_MapSceneTemp.emplace(SCENETAG::LOBBY, pScene);

				Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);
				break;
			}
			case CLoading::LOADING_BOSS:
			{
				CScene* pScene = CBossStage::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);

				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
				FAILED_CHECK_RETURN(Engine::COctree::GetInstance()->Ready_Octree(), E_FAIL);

				map<SCENETAG, CScene*>		m_MapSceneTemp;
				m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();
				pScene->Set_SceneTag(SCENETAG::BOSS_STAGE);
				m_MapSceneTemp.emplace(SCENETAG::BOSS_STAGE, pScene);
				Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);

				break;
			}
			}
		}
	}


	return iExit;
}

void CLoadingStage1::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLoadingStage1::Render_Scene()
{
	//렌더는 렌더러에서 관리하기 때문에 굳이 여기에 넣을 필요 없음.
}

HRESULT CLoadingStage1::Ready_Prototype()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UITex", CUITex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoCharacterTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Main menu characters1.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoTitleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/logo.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ProgressBarFrameTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/ProgressBar/ProgressBarEmpty.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ProgressBarValueTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/ProgressBar/ProgressBarFull.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CameraCom", CCameraCom::Create(m_pGraphicDev)), E_FAIL); // 유진 추가

	return S_OK;
}

HRESULT CLoadingStage1::Ready_Layer_Environment(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	// Background
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BACKGOUND, pGameObject), E_FAIL);

	// Title
	pGameObject = CTitle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BACKGOUND, pGameObject), E_FAIL);

	pGameObject = CProgressValue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PROGRESSVALUE, pGameObject), E_FAIL);

	pGameObject = CProgressFrame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PROGRESSFRAME, pGameObject), E_FAIL);

	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

CLoadingStage1* CLoadingStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag)
{
	CLoadingStage1* pInstance = new CLoadingStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(eSceneTag)))
	{
		Safe_Release(pInstance);
		MSG_BOX("LoadingStage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoadingStage1::Free()
{
	Safe_Release(m_pLoading);

	__super::Free();
}
