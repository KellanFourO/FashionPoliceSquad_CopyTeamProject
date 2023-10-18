#include "..\..\Header\Layer.h"
#include "Export_Utility.h"

CLayer::CLayer()
{
}

CLayer::CLayer(LAYERTAG _eLayerTag)
{
}


CLayer::~CLayer()
{
}

CComponent* CLayer::Get_Component(COMPONENTID eID, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_FinderEnum<OBJECTTAG>(eObjTag));

	if (iter == m_mapObject.end())
		return nullptr;


	return iter->second.front()->Get_Component(eID, eComponentTag);
}

HRESULT CLayer::Add_GameObject(OBJECTTAG eObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject[eObjTag].push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(OBJECTTAG eObjTag, CGameObject* pGameObject, _uint pOBJIndex)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	/*for (int i = 0; i < m_mapObject[eObjTag].size(); ++i)
	{
		if (m_mapObject[eObjTag][i]->m_iIndex == pOBJIndex)
		{
			Safe_Release(m_mapObject[eObjTag][i]);

			break;
		}
	}*/

	for (auto& iter : m_mapObject) {
		auto& objectVector = iter.second; // 현재 엔트리의 객체 벡터에 대한 참조를 얻습니다.

		for (auto iter2 = objectVector.begin(); iter2 != objectVector.end();) {

			if ((*iter2)->Get_iIndex() == pOBJIndex) {
				Safe_Release(*iter2);
				iter2 = objectVector.erase(iter2); // erase 함수로 현재 요소를 삭제하고 다음 요소로 이동합니다.
			}
			else {
				++iter2; // 삭제하지 않는 경우에는 다음 요소로 이동합니다.
			}
		}
	}

	return S_OK;
}


HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int iResult = 0;


 	for (auto& iter : m_mapObject)
 	{


 		for (auto iter2 = iter.second.begin(); iter2 != iter.second.end();)
 		{
			//todo iter2 vector<GameObject*>
 			if (*iter2 != nullptr)
 			{

 				iResult = (*iter2)->Update_GameObject(fTimeDelta);


 				if (iResult == OBJ_DEAD)
 				{
 					Safe_Release(*iter2);
 					iter2 = iter.second.erase(iter2);
 				}
 				else
 				{
 					++iter2;
 				}

 				if (iResult & 0x80000000)
 					return iResult;
 			}
 			else
 			{
 				iter2 = iter.second.erase(iter2);
 			}
 		}
 	}
	return iResult;
	//for (auto& iter : m_mapObject)
	//{
	//	for (auto& iter2 : iter.second)
	//	{
	//		iResult = iter2->Update_GameObject(fTimeDelta);
	//	}
	//	if (iResult & 0x80000000)
	//		return iResult;
	//}
	//return iResult;

}



void CLayer::LateUpdate_Layer()
{
	for (auto& iter : m_mapObject)
	{
		for (auto& iter2 : iter.second)
		{
			iter2->LateUpdate_GameObject();
		}
	}

}

CGameObject* CLayer::Find_GameObject(OBJECTTAG eObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_FinderEnum<OBJECTTAG>(eObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second.front();
}




CLayer* CLayer::Create(LAYERTAG _eLayerTag)
{
	CLayer* pLayer = new CLayer(_eLayerTag);

	if (FAILED(pLayer->Ready_Layer()))
	{
		Safe_Release(pLayer);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pLayer;
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer();

	if (FAILED(pLayer->Ready_Layer()))
	{
		Safe_Release(pLayer);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
 	//for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteVector());
	//m_mapObject.clear();
}
