#include "stdafx.h"
#include "Engine_Define.h"

#include "MapTool.h"
#include "ImGuiManager.h"

#include "Export_System.h"
#include "Export_Utility.h"

CMapTool::CMapTool(LPDIRECT3DDEVICE9 pGraphicDev) 
	: Engine::CScene(pGraphicDev), m_vCursor_Pos(&_vec3())
{
}  

CMapTool::~CMapTool()
{
	Free();
}

HRESULT CMapTool::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	return S_OK;
}

HRESULT CMapTool::Ready_LightInfo()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//mainApp ���� ���⿡�� �ѱ�
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f }; // ����(���ݻ�)
	tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f }; // ���ݻ�
	tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f }; //���� ���� ȯ��ݻ�
	tLightInfo.Direction = { 1.f, -1.f, 1.f }; //����

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}


_int CMapTool::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);
	
	BUILD_NOP_MODE();
	Build_Map();

	if (CImGuiManager::GetInstance()->Get_Load_Check() == true)
	{
		Load_Cube(L"../Bin/Data/Map/MapData");
		CImGuiManager::GetInstance()->Set_Load_Check();
	}
	return iExit;
}


void CMapTool::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	if (CImGuiManager::GetInstance()->Get_DeleteMode_Check() == true)
		{	Delete_Map();	}

}

void CMapTool::Render_Scene()
{
	// -DEBUG �� 
}


HRESULT CMapTool::Ready_Layer_Environment(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = m_pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f), //�� �ָ� ���� ��� -10
		&_vec3(0.f, 0.f, 1.f),    //Z���� 1�� �ʱ�ȭ �ؾ��ϹǷ� At�� 0,0,1
		&_vec3(0.f, 1.f, 0.f));   //Up�� Y�� �ʱ�ȭ�̹Ƿ� 0,1,0
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::DCAMERA, pGameObject), E_FAIL);

	// FlyingCamera
// 	pGameObject = CFlyingCamera::Create(m_pGraphicDev);
// 	NULL_CHECK_RETURN(pGameObject, E_FAIL);
// 	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	//WireFrame
	pGameObject = CWireFrame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::WIREFRAME, pGameObject), E_FAIL);

	//MapCursor
	pGameObject = m_pMapCursor = CMapCursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MAPCURSOR, pGameObject), E_FAIL);

 	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

void CMapTool::BUILD_NOP_MODE()
{
	if (Engine::Get_DIKeyState(DIK_F7)) 
	{
		if (true == m_BUILD_NOP_MODE)
			m_BUILD_NOP_MODE = false;

		else if (false == m_BUILD_NOP_MODE)
			m_BUILD_NOP_MODE = true;
	}

}


void CMapTool::Cursor_Update()
{
	m_vCursor_Pos	= m_pMapCursor->Get_MapCursor_Pos();
	m_vCursor_Size	= { m_pMapCursor->Get_MapCursorSize().m_fX,
						m_pMapCursor->Get_MapCursorSize().m_fY,
						m_pMapCursor->Get_MapCursorSize().m_fZ };
	m_vCursor_Height = CImGuiManager::GetInstance()->Get_CubeHeightLevel();
}

void CMapTool::CubeSize_Update()
{
	m_fCubesize.m_fX = CImGuiManager::GetInstance()->Get_CubeSize().m_fX;
	m_fCubesize.m_fY = CImGuiManager::GetInstance()->Get_CubeSize().m_fY;
	m_fCubesize.m_fZ = CImGuiManager::GetInstance()->Get_CubeSize().m_fZ;
}


HRESULT CMapTool::Build_Map() //Cube�ų� OBJ �ų�
{
	if (false == m_BUILD_NOP_MODE) {


		if ((Engine::Get_DIMouseState(DIM_RB)) && (CImGuiManager::GetInstance()->Get_DeleteMode_Check() == false))
		{
			Engine::CGameObject* pGameObject = nullptr;

			if (CImGuiManager::GetInstance()->Get_BuildModeCheck() == true) {
				CubeTemp2 = new CUBE;
			}
			else if (CImGuiManager::GetInstance()->Get_OBJModeCheck() == true) {
				OBJTemp = new OBJData;
			}


				//////////////////���� �κ�/////////////////////
				Cursor_Update();
				_vec3 CursorTemp = { m_vCursor_Pos->x, m_vCursor_Height, m_vCursor_Pos->z };

			if ((!CheckDuplicateCube(CursorTemp, m_vCursor_Size))  
				&& (CursorTemp.x > -1)	&& (CursorTemp.y > -1)
				&& (CursorTemp.z > -1)	&& (m_Build_time_Check == false))
			{
				m_Build_time_Check = true;
				
				if (CImGuiManager::GetInstance()->Get_NotNormal_Check() == false)
				{	m_fCubesize = { VTXITV, VTXITV, VTXITV };	}

				else if (CImGuiManager::GetInstance()->Get_NotNormal_Check() == true)
				{	CubeSize_Update();	}

				_vec3 CubeSize, ObjSize;
				CubeSize = ObjSize = { m_fCubesize.m_fX, m_fCubesize.m_fY, m_fCubesize.m_fZ };
				///////////////////////////////////////////////



				//�� �༮�� Build_Cube�� �ش�� ���
				if (CImGuiManager::GetInstance()->Get_BuildModeCheck() == true) {  
					m_iTextureNum = CImGuiManager::GetInstance()->Get_CubeTexNum();
					pGameObject = CBuild_Cube::Create(m_pGraphicDev, CursorTemp, m_iTextureNum, CubeSize, m_iCubeIndex);

					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_CUBE, pGameObject), E_FAIL);

					m_mapLayer.insert({ LAYERTAG::ENVIRONMENT, m_pLayer });

					CubeTemp2->vSize = CubeSize;
					CubeTemp2->m_OBJ_TYPE = OBJ_TYPE::BUILDING;
					CubeTemp2->uITextureNum = m_iTextureNum;
					CubeTemp2->vPos = CursorTemp;
					CubeTemp2->iCubeIndex = m_iCubeIndex;

					m_VecCubeData.push_back(CubeTemp2);
					m_iCubeIndex++;
				}

				//�� �༮�� Build_OBJ�� �ش�� ���
				else if (CImGuiManager::GetInstance()->Get_OBJModeCheck() == true) { 
					m_iTextureNum2 = CImGuiManager::GetInstance()->Get_OBJTexNum();
					OBJ_TYPE eTypeTemp = CImGuiManager::GetInstance()->Get_OBJType(); //������Ʈ �±׶� �ٸ�����
					_uint RotateCount = CImGuiManager::GetInstance()->Get_OBJ_RotateCountCW();

					pGameObject = CBuild_Obj::Create(m_pGraphicDev, CursorTemp, m_iTextureNum2, ObjSize, RotateCount, m_iOBJIndex, eTypeTemp);

					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_OBJ, pGameObject), E_FAIL);

					m_mapLayer.insert({ LAYERTAG::ENVIRONMENT, m_pLayer });

					OBJTemp->vSize = ObjSize;
					OBJTemp->m_OBJ_TYPE = CImGuiManager::GetInstance()->Get_OBJType();
					OBJTemp->uITextureNum = m_iTextureNum2;
					OBJTemp->vPos = CursorTemp;
					OBJTemp->iIndex = m_iOBJIndex;
					OBJTemp->iRotateCount = RotateCount;

					if (eTypeTemp == OBJ_TYPE::CUBE_OBJ)
					{
						m_VecTempCube.clear();
						m_VecTempCube = CImGuiManager::GetInstance()->Get_CubeTextureObjVector();
						OBJTemp->pCubeTexture = m_VecTempCube[m_iTextureNum2 - cubeObjTextureStartIndex];
					}
					else if (eTypeTemp == OBJ_TYPE::PLANE_OBJ)
					{
						m_VecTempPlane.clear();
						m_VecTempPlane = CImGuiManager::GetInstance()->Get_PlaneTextureObjVector();
						OBJTemp->pBaseTexture = m_VecTempPlane[m_iTextureNum2 - planeObjTextureStartIndex];
					}
					m_VecOBJData.push_back(OBJTemp);
					m_iOBJIndex++;
				}
			}

			if (m_Build_time_Check == true)
			{
				m_Build_time++;
				
				if (m_Build_time >= 5) 
				{
					m_Build_time = 0;
					m_Build_time_Check = false;
				}
			}
		}
	}	
	return S_OK;
}

/*
	Safe_Release ref ī���Ͱ� �ö󰡴µ� �̹� �����µ� �� �ҷ��ų�, 
	���� ī���� �ø����ʾҴµ� �ҷ�
*/


HRESULT CMapTool::Load_Cube(const TCHAR* pFilePath)
{
	//���� �����ؼ� �޾ƿ���
	string m_strText = "MapData";

	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD   dwByte = 0;
	DWORD   dwStrByte = 0;
	CUBE* pCube = nullptr;

	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	pTag = new CHAR[dwStrByte];

	ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
	m_strText = pTag;

	basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());

	//����� �����ʹ�� ť�� �����Ҵ��ؼ� ���Ϳ� ���
	while (true)
	{
		pCube = new CUBE;

		ReadFile(hFile, pCube, sizeof(CUBE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pCube);
			break;
		}

		m_VecCubeData.push_back(pCube);
	}
	CloseHandle(hFile);

	Engine::CGameObject* pGameObject = nullptr;

	//���� ���빰��ŭ ���� ť�� ������ ���̾ ���
	for (auto& iter : m_VecCubeData)
	{
		pGameObject = CBuild_Cube::Create(m_pGraphicDev, iter->vPos, iter->uITextureNum, iter->vSize, m_iCubeIndex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_CUBE, pGameObject), E_FAIL);
		m_iCubeIndex++;
	}
	m_mapLayer.insert({ LAYERTAG::ENVIRONMENT, m_pLayer });

	delete[] pTag;
	pTag = nullptr;

	MSG_BOX("Load Complete.");
	return S_OK;
}

HRESULT CMapTool::Delete_Map()
{
	// 2�� ���Ϳ��� �����.
	// 1. ������ ������ �ִ� SAVE-LOAD�� ����
	// 2. Create �� �����Ǿ� Layer�� ���� �Ǵ� GameObject ����

	if (Engine::Get_DIMouseState(DIM_RB))
	{
		Cursor_Update();
		_vec3 CursorTemp = { m_vCursor_Pos->x, m_vCursor_Height, m_vCursor_Pos->z };
		_uint IndexTemp = -1;

		if (CImGuiManager::GetInstance()->Get_BuildModeCheck() == true) {

			for (auto iter = m_VecCubeData.begin(); iter != m_VecCubeData.end();)
			{
				if (fabsf((*iter)->vPos.x - CursorTemp.x) < VTXITV &&
					fabsf((*iter)->vPos.y - CursorTemp.y) < VTXITV &&
					fabsf((*iter)->vPos.z - CursorTemp.z) < VTXITV)
				{
					IndexTemp = (*iter)->iCubeIndex;

					delete *iter;
					//*iter = nullptr;
					iter = m_VecCubeData.erase(iter);


					if (IndexTemp != -1) {
						auto& VectorTemp = Engine::Management()->GetInstance()->Get_Scene()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::BUILD_CUBE);

						for (int i = 0; i < VectorTemp.size(); ++i)
						{
							if (IndexTemp == VectorTemp[i]->Get_iIndex())
							{
								m_pLayer->Delete_GameObject(OBJECTTAG::BUILD_CUBE, VectorTemp[i], IndexTemp);
								break;
							}
						}
					}

					//iter = m_VecCubeData.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}

		if (CImGuiManager::GetInstance()->Get_OBJModeCheck() == true)
		{
			for (auto iter = m_VecOBJData.begin(); iter != m_VecOBJData.end();)
			{
				if (fabsf((*iter)->vPos.x - CursorTemp.x) < VTXITV &&
					fabsf((*iter)->vPos.y - CursorTemp.y) < VTXITV &&
					fabsf((*iter)->vPos.z - CursorTemp.z) < VTXITV)
				{
					IndexTemp = (*iter)->iIndex;

					delete *iter;
					//*iter = nullptr;
					iter = m_VecOBJData.erase(iter);

					if (IndexTemp != -1) {
						auto& ObjVectorTemp = Engine::Management()->GetInstance()->Get_Scene()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::BUILD_OBJ);

						for (int i = 0; i < ObjVectorTemp.size(); ++i)
						{
							if (IndexTemp == ObjVectorTemp[i]->Get_iIndex())
							{
								m_pLayer->Delete_GameObject(OBJECTTAG::BUILD_OBJ, ObjVectorTemp[i], IndexTemp);
								break;
							}
						}
					}

					//iter = m_VecOBJData.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}
	return S_OK;
}

bool CMapTool::CheckDuplicateCube(const _vec3& pPos, const _vec3& pSize)
{
	if (m_VecCubeData.empty())
		return false;
		
	bool bCheckPoint = true; //�� �� ���µ��� ��� true���(�ѹ��� �� �ɸ���) �װ� �� �ߺ�

	for (const auto& iter : m_VecCubeData)
	{
		if (bCheckPoint == false) // �ߺ��� �� �߻������ϱ� �� ���� �ٽ� ���ƾ���
			{ bCheckPoint = true; } //Ʈ��� ����� �ٽ� ����

		_vec3 minPoint1 = pPos - pSize * 0.5f;  //���� ���Ϸ��� �ϴ� ��
		_vec3 maxPoint1 = pPos + pSize * 0.5f;

		_vec3 minPoint2 = iter->vPos - iter->vSize * 0.5f; //���� ��ġ�� ��
		_vec3 maxPoint2 = iter->vPos + iter->vSize * 0.5f;

		//���� AABB ��Ģ�� ���� �ߺ��˻� ��ƾ!!!! ���⿡ �ɸ��� �ߺ��� �ƴѰ���

		float max_min = max(minPoint1.x, minPoint2.x);
		float min_max = min(maxPoint1.x, maxPoint2.x);
		bool result1 = max(minPoint1.x, minPoint2.x) >= min(maxPoint1.x, maxPoint2.x);

		if (max(minPoint1.x, minPoint2.x) >= min(maxPoint1.x, maxPoint2.x))
		{
			bCheckPoint = false; //x ���� �ߺ��� �ƴϴ�
		}

		float max_min2 = max(minPoint1.y, minPoint2.y);
		float min_max2 = min(maxPoint1.y, maxPoint2.y);
		bool result2 = max(minPoint1.y, minPoint2.y) >= min(maxPoint1.y, maxPoint2.y);

		if (max(minPoint1.y, minPoint2.y) >= min(maxPoint1.y, maxPoint2.y))
		{
			bCheckPoint = false; //y ���� �ߺ��� �ƴϴ�
		}

		float max_min3 = max(minPoint1.z, minPoint2.z);
		float min_max3 = min(maxPoint1.z, maxPoint2.z);
		bool result3 = max(minPoint1.z, minPoint2.z) >= min(maxPoint1.z, maxPoint2.z);

		if (max(minPoint1.z, minPoint2.z) >= min(maxPoint1.z, maxPoint2.z))
		{
			bCheckPoint = false; //z ���� �ߺ��� �ƴϴ�
		}

		if (bCheckPoint == true) //Ʈ��� �����ؼ� �� �״�� �Դٴ°� �ϳ��� �� �ɸ� ��
			{ return true; }  // �� �ߺ��� �߻��� ���� �׷� �ٷ� return�Ѵ�		
	}

	return false; // �� ���ƾ� ������! = ��ġ ����/ �ߺ� �ƴ�
}

CMapTool* CMapTool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapTool* pInstance = new CMapTool(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MapTool Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMapTool::Free()
{
	if (pTag)
	{
		delete[] pTag;
		pTag = nullptr;
	}

	Safe_Delete(OBJTemp);
	Safe_Delete(CubeTemp2);

	if (!m_VecCubeData.empty()) {
		for (int i = 0; i < m_VecCubeData.size(); ++i)
		{
			Safe_Delete(m_VecCubeData[i]);
		}
		m_VecCubeData.clear();
	}

	if (!m_VecOBJData.empty()) {
		for (int i = 0; i < m_VecOBJData.size(); ++i)
		{
			Safe_Delete(m_VecOBJData[i]);
		}
		m_VecOBJData.clear();
 	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	__super::Free();
}
