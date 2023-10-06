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

HRESULT CLoadingStage1::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);

	if (m_pScene->Get_SceneTag() == SCENETAG::STAGE2)
	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	}
	else if (m_pScene->Get_SceneTag() == SCENETAG::BOSS_STAGE)
	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	}
	else if (m_pScene->Get_SceneTag() == SCENETAG::LOBBY)
	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
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
			if (m_pScene->Get_SceneTag() == SCENETAG::BOSS_STAGE)
			{
				CScene* pScene = CBossStage::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);
				pScene->Set_MainPlayer(m_pPlayer);
				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
				FAILED_CHECK_RETURN(Engine::COctree::GetInstance()->Ready_Octree(), E_FAIL);


				m_MapSceneTemp = Management()->Get_MapScene();
				pScene->Set_SceneTag(SCENETAG::BOSS_STAGE);
				m_MapSceneTemp.emplace(SCENETAG::BOSS_STAGE, pScene);
				Management()->Set_MapScene(m_MapSceneTemp);
			}
			else if (m_pScene->Get_SceneTag() == SCENETAG::LOBBY)
			{

			}
			else if (m_pScene->Get_SceneTag() == SCENETAG::STAGE2)
			{

			}
		
		}
	}


	return;
}

void CLoadingStage1::LateUpdate_Scene()
{
}

void CLoadingStage1::Render_Scene()
{
}

HRESULT CLoadingStage1::Ready_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CLoadingStage1::Ready_Layer_Environment(LAYERTAG eLayerTag)
{
	return E_NOTIMPL;
}

CLoadingStage1* CLoadingStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CLoadingStage1::Free()
{
}
