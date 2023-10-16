#include "..\..\Header\Scene.h"
#include "Octree.h"
#include "Export_Utility.h"
#include "../Client/Header/NewFPSCamera.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev),
	  m_eSceneTag(SCENETAG::SCENETAG_END),
	  m_bPause(false),m_bReadyCube(false),m_pPlayer(nullptr)
{

	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{

}

CComponent* Engine::CScene::Get_Component(COMPONENTID eID, LAYERTAG eLayerTag, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_FinderEnum<LAYERTAG>(eLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(eID, eObjTag, eComponentTag);
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
	_int	iResult = 0;

	for (auto& iter : m_mapLayer)
	{

		if (iter.first == LAYERTAG::ENVIRONMENT && !m_bReadyCube)
		{
			CLayer* pLayer = iter.second;
			if (!pLayer->Get_ObjectList(OBJECTTAG::BUILD_CUBE).empty())
			{
				//COctree::GetInstance()->Ready_Octree();
				m_bReadyCube = true;
			}

		}

		if (m_bPause)
		{
			if (iter.first == LAYERTAG::GAMELOGIC)
			{
				for (auto Objiter : iter.second->Get_mapObject())
				{
					for (int i = 0; i < Objiter.second.size(); ++i)
					{
						if (Objiter.second[i]->Get_ObjectTag() == OBJECTTAG::BOSS)
						{
							Objiter.second[i]->Update_GameObject(fTimeDelta);
							Objiter.second[i]->LateUpdate_GameObject();
						}

						if (Objiter.second[i]->Get_ObjectTag() == OBJECTTAG::MINIGAME)
						{
							Objiter.second[i]->Update_GameObject(fTimeDelta);
							Objiter.second[i]->LateUpdate_GameObject();
						}


						Renderer()->Add_RenderGroup(RENDER_ALPHATEST, Objiter.second[i]);
						Objiter.second[i]->Render_GameObject();
					}
				}
			}
			else if (iter.first == LAYERTAG::CAMERA)
			{

				dynamic_cast<CNewFPSCamera*>(iter.second->Get_ObjectList(OBJECTTAG::FPSCAMERA).back())->Set_Fix(false);
				iResult = iter.second->Update_Layer(fTimeDelta);
			}
			else
			{

				iResult = iter.second->Update_Layer(fTimeDelta);
			}
		}
		else
		{
			if (iter.first == LAYERTAG::CAMERA)
			{
			dynamic_cast<CNewFPSCamera*>(iter.second->Get_ObjectList(OBJECTTAG::FPSCAMERA).back())->Set_Fix(true);
			}
			iResult = iter.second->Update_Layer(fTimeDelta);
		}

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::LateUpdate_Scene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();


}

void CScene::Render_Scene()
{
	// µð¹ö±ë¿ë
}

void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}

