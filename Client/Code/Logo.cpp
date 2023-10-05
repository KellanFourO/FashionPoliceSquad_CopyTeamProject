#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Stage.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev) , m_pLoading(nullptr)
	, m_pLoadingID(CLoading::LOADING_END)
	, m_CheckPoint(0)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);



	m_CheckPoint = MessageBox(g_hWnd,
		L"확인 선택 시 스테이지, 취소 선택 시 맵 툴이 실행됩니다.",
		L"실행 조건 확인", MB_OKCANCEL);

	if (m_CheckPoint == IDOK)	{
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	}

	else if (m_CheckPoint == IDCANCEL) {
		m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_MAPTOOL);
	}
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);



	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
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

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{

			if (m_pLoadingID == CLoading::LOADING_MAPTOOL) {
				CScene* pScene = CMapTool::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);


				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

				map<SCENETAG, CScene*>		m_MapSceneTemp;
				m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();

				pScene->Set_SceneTag(SCENETAG::MAPTOOL);
				m_MapSceneTemp.emplace( SCENETAG::MAPTOOL , pScene );

				Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);

				return 0;
			}

			if (m_pLoadingID == CLoading::LOADING_STAGE) {
				CScene* pScene = CStage::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);

				pScene->Set_MainPlayer(m_pPlayer);

				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

				FAILED_CHECK_RETURN(Engine::COctree::GetInstance()->Ready_Octree(), E_FAIL);

				map<SCENETAG, CScene*>		m_MapSceneTemp;
				m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();
				pScene->Set_SceneTag(SCENETAG::STAGE);
				m_MapSceneTemp.emplace(SCENETAG::STAGE, pScene);
				Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);

				return 0;
			}


		}
	}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	// Debug

}

HRESULT CLogo::Ready_Prototype()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UITex", CUITex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoCharacterTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Main menu characters1.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoTitleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/logo.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ProgressBarFrameTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/ProgressBar/ProgressBarEmpty.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ProgressBarValueTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/ProgressBar/ProgressBarFull.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CameraCom", CCameraCom::Create(m_pGraphicDev)), E_FAIL); // 유진 추가

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment(LAYERTAG eLayerTag)
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

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo* pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free()
{

	Safe_Release(m_pLoading);

	__super::Free();
}
