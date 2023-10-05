#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr),m_bSceneChange(nullptr)
{
	for (_uint i = 0; (SCENETAG)i < SCENETAG::SCENETAG_END; ++i)
	{
		m_bStageVisit[i] = false;
	}
}


CManagement::~CManagement()
{
	Free();
}

CComponent* Engine::CManagement::Get_Component(COMPONENTID eID, LAYERTAG eLayerTag, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(eID, eLayerTag, eObjTag, eComponentTag);
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
	Safe_Release(m_pScene);

	Engine::Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

HRESULT CManagement::Change_Scene(CScene* pScene)
{
	m_bSceneChange = true;

	m_pPreScene = m_pScene;
	m_pNextScene = pScene;

	// 이전 씬을 방문 여부를 기록
	m_bStageVisit[(_uint)pScene->Get_SceneTag() - 1] = true;
	return S_OK;
}

HRESULT CManagement::Load_Data()
{
	if (m_pScene)
		m_pScene->Load_Data();

	return S_OK;
}

_int CManagement::Update_Scene(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pScene, -1)
		return m_pScene->Update_Scene(fTimeDelta);

	if (m_bSceneChange)
	{
		Octree()->Ready_Octree();
		m_bSceneChange = false;
	}

	Octree()->Update_Octree();
}

void CManagement::LateUpdate_Scene()
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK(m_pScene);
	m_pScene->Render_Scene();

	if (SCENETAG::STAGE == m_pScene->Get_SceneTag())
		Octree()->Render_Octree(pGraphicDev);

	Renderer()->Render_GameObject(pGraphicDev);


}

void CManagement::Free()
{
	Safe_Release(m_pScene);
}
