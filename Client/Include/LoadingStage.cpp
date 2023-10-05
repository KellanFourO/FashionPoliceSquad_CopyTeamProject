#include "stdafx.h"
#include "LoadingStage.h"
#include "Stage.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MyUI.h"

CLoadingStage::CLoadingStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{
}

CLoadingStage::~CLoadingStage()
{
}

HRESULT CLoadingStage::Ready_Scene()
{
	m_eSceneTag = SCENETAG::LOADING;
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	map<SCENETAG, CScene*>		m_MapSceneTemp;
// 	m_MapSceneTemp = Engine::CManagement::GetInstance()->Get_MapScene();
// 	m_MapSceneTemp.emplace(SCENETAG::STAGE, pScene);
// 	Engine::CManagement::GetInstance()->Set_MapScene(m_MapSceneTemp);
	return S_OK;
}

_int CLoadingStage::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Scene(fTimeDelta);

	

	//Loading이 다되었으면~ 어케 하겠다
	if (true == m_pLoading->Get_Finish())
	{
		CScene* pScene = CStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);
		FAILED_CHECK_RETURN(Engine::Management()->Set_Scene(pScene), E_FAIL);
		Management()->Load_Data();
		Octree()->Ready_Octree();
	}

	return iExit;
}

void CLoadingStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

}

void CLoadingStage::Render_Scene()
{

}

HRESULT CLoadingStage::Ready_Prototype()
{
	return S_OK;
}

HRESULT CLoadingStage::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	return S_OK;
}

CLoadingStage* CLoadingStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingStage* pInstance = new CLoadingStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;

}

void CLoadingStage::Free()
{
	Safe_Release(m_pLoading);

	__super::Free();
}
