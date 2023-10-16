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
}

HRESULT CMapTool::Ready_Scene()
{
    FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);

    FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

    return S_OK;
}

HRESULT CMapTool::Ready_LightInfo()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    //mainApp 말고 stage-Maptool 여기에서 켜기
    //m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

    D3DLIGHT9 tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

    tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f }; // 색깔(난반사)
    tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f }; // 정반사
    tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f }; //조명에 의한 환경반사
    tLightInfo.Direction = { 1.f, -1.f, 1.f }; //방향

    FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

    return S_OK;
}


_int CMapTool::Update_Scene(const _float& fTimeDelta)
{
    _int   iExit = __super::Update_Scene(fTimeDelta);

    BUILD_NOP_MODE();
    Build_Map();

    if (CImGuiManager::GetInstance()->Get_Load_Check() == true)
    {
        Load_Data(L"../Bin/Data/Map/MapData", OBJECTTAG::BUILD_CUBE);
        CImGuiManager::GetInstance()->Set_Load_Check();
    }
    else if (CImGuiManager::GetInstance()->Get_OBJLoad_Check() == true)
    {
        Load_Data(L"../Bin/Data/OBJ/OBJData", OBJECTTAG::BUILD_OBJ);
        CImGuiManager::GetInstance()->Set_OBJLoad_Check();
    }
    else if (CImGuiManager::GetInstance()->Get_CPOINT_Load_Check() == true)
    {
        Load_Data_C(L"../Bin/Data/CPoint/CPointData", OBJECTTAG::BUILD_OBJ);
        CImGuiManager::GetInstance()->Set_CPOINT_Load_Check();
    }
    else if (CImGuiManager::GetInstance()->Get_TriggerLoad_Check() == true)
    {
        Load_Data_T(L"../Bin/Data/Trigger/TriggerData", OBJECTTAG::O_TRIGGER);
        CImGuiManager::GetInstance()->Set_TriggerLoad_Check();
    }


    return iExit;
}


void CMapTool::LateUpdate_Scene()
{
    __super::LateUpdate_Scene();

    if (CImGuiManager::GetInstance()->Get_DeleteMode_Check() == true)
    {
        Delete_Map();
    }

}

void CMapTool::Render_Scene()
{
    // -DEBUG 등
}


HRESULT CMapTool::Ready_Layer_Environment(LAYERTAG eLayerTag)
{
    Engine::CLayer* pLayer = m_pLayer = Engine::CLayer::Create(eLayerTag);
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CDynamicCamera::Create(m_pGraphicDev,
        &_vec3(0.f, 10.f, -10.f), //좀 멀리 배율 잡게 -10
        &_vec3(0.f, 0.f, 1.f),    //Z값은 1로 초기화 해야하므로 At은 0,0,1
        &_vec3(0.f, 1.f, 0.f));   //Up은 Y값 초기화이므로 0,1,0
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::DCAMERA, pGameObject), E_FAIL);

    // FlyingCamera
 //    pGameObject = CFlyingCamera::Create(m_pGraphicDev);
 //    NULL_CHECK_RETURN(pGameObject, E_FAIL);
 //    pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

    //WireFrame
    pGameObject = CWireFrame::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::WIREFRAME, pGameObject), E_FAIL);

    m_mapLayer.emplace( eLayerTag, pLayer );

    return S_OK;
}

HRESULT CMapTool::Ready_Layer_GameLogic(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = m_pGLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;


	//MapCursor
	pGameObject = m_pMapCursor = CMapCursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MAPCURSOR, pGameObject), E_FAIL);


	// Trigger
    //Load_Trigger(L"../Bin/Data/Trigger/TriggerData");

	m_mapLayer.emplace(eLayerTag, pLayer);

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
    m_vCursor_Pos = m_pMapCursor->Get_MapCursor_Pos();
    m_vCursor_Size = { m_pMapCursor->Get_MapCursorSize().fX,
                       m_pMapCursor->Get_MapCursorSize().fY,
                       m_pMapCursor->Get_MapCursorSize().fZ };
    m_vCursor_Height = CImGuiManager::GetInstance()->Get_CubeHeightLevel();
}

void CMapTool::CubeSize_Update()
{
    m_fCubesize.fX = CImGuiManager::GetInstance()->Get_CubeSize().fX;
    m_fCubesize.fY = CImGuiManager::GetInstance()->Get_CubeSize().fY;
    m_fCubesize.fZ = CImGuiManager::GetInstance()->Get_CubeSize().fZ;
}


HRESULT CMapTool::Build_Map() //Cube거나 OBJ 거나
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
                OBJ_C_POINT = new C_POINT;
            }
			else if (CImGuiManager::GetInstance()->Get_TriggerMode_Check() == true) {
                TriggerTemp = new TRIGGER;
			}

            Cursor_Update();
            _vec3 CursorTemp = { m_vCursor_Pos->x, m_vCursor_Height, m_vCursor_Pos->z };

            if ((!CheckDuplicateCube(CursorTemp, m_vCursor_Size))
                && (CursorTemp.x > -1) && (CursorTemp.y > -1)
                && (CursorTemp.z > -1) && (m_Build_time_Check == false))
            {
                m_Build_time_Check = true;

                if (CImGuiManager::GetInstance()->Get_NotNormal_Check() == false)
                {
                    m_fCubesize = { VTXITV, VTXITV, VTXITV };
                }
                else if (CImGuiManager::GetInstance()->Get_NotNormal_Check() == true)
                {
                    CubeSize_Update();
                }

                _vec3 CubeSize, ObjSize;
                CubeSize = ObjSize = { m_fCubesize.fX, m_fCubesize.fY, m_fCubesize.fZ };

                //이 녀석이 Build_Cube에 해당될 경우
                if (CImGuiManager::GetInstance()->Get_BuildModeCheck() == true) {
                    m_iTextureNum = CImGuiManager::GetInstance()->Get_CubeTexNum();
                    pGameObject = CBuild_Cube::Create(m_pGraphicDev, CursorTemp, m_iTextureNum, CubeSize, m_iCubeIndex);

                    NULL_CHECK_RETURN(pGameObject, E_FAIL);
                    FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_CUBE, pGameObject), E_FAIL);

                    m_mapLayer.emplace( LAYERTAG::ENVIRONMENT, m_pLayer );

                    CubeTemp2->vSize = CubeSize;
                    CubeTemp2->eOBJ_TYPE = OBJ_TYPE::BUILDING_TYPE;
                    CubeTemp2->uITextureNum = m_iTextureNum;
                    CubeTemp2->vPos = CursorTemp;
                    CubeTemp2->iCubeIndex = m_iCubeIndex;

                    m_VecCubeData.push_back(CubeTemp2);
                    m_iCubeIndex++;
                }

				//이 녀석이 Trigger 에 해당될 경우
                if ((CImGuiManager::GetInstance()->Get_TriggerMode_Check() == true)
                    && (CImGuiManager::GetInstance()->Get_TriggerReady_Check() == true))
                {
                    TRIGGER_CASE		m_eTRCase = CImGuiManager::GetInstance()->Get_ImGuiTriggerCase();
                    TRIGGER_TYPE		m_eTRType = CImGuiManager::GetInstance()->Get_ImGuiTriggerType();
					TRIGGER_NUMBER		m_eTrName = CImGuiManager::GetInstance()->Get_ImGuiTriggerNumber();

					pGameObject = CTrigger::Create(m_pGraphicDev, CursorTemp, m_iTriggerIndex, CubeSize, m_eTRCase, m_eTRType, m_eTrName);

					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(m_pGLayer->Add_GameObject(OBJECTTAG::O_TRIGGER, pGameObject), E_FAIL);

                    m_mapLayer.emplace( LAYERTAG::GAMELOGIC, m_pGLayer);

                    TriggerTemp->eTrName = m_eTrName;
                    TriggerTemp->vSize = CubeSize;
                    TriggerTemp->vPos = CursorTemp;
                    TriggerTemp->iIndex = m_iTriggerIndex;
                    TriggerTemp->eTrCase = m_eTRCase;
                    TriggerTemp->eTrType = m_eTRType;
                    TriggerTemp->eTrSTATE = TRIGGER_STATE::TR_BEFORE;

                    m_TriggerData.push_back(TriggerTemp);
                    CImGuiManager::GetInstance()->Set_TriggerReady_Check();
                    ++m_iTriggerIndex;
				}
            }

            //지금은 중복 검사하는 쪽으로 설정, 중복 불가하게 하고 싶으면 첫 줄 주석
            if (/*(!CheckDuplicateCube(CursorTemp, m_vCursor_Size) &&   */             
                (CursorTemp.x > -1) && (CursorTemp.y > -1)
                && (CursorTemp.z > -1) && (m_Build_time_Check2 == false))
            {
                m_Build_time_Check2 = true;

                if (CImGuiManager::GetInstance()->Get_NotNormal_Check() == false)
                {
                    m_fCubesize = { VTXITV, VTXITV, VTXITV };
                }
                else if (CImGuiManager::GetInstance()->Get_NotNormal_Check() == true)
                {
                    CubeSize_Update();
                }

                _vec3 CubeSize, ObjSize;
                CubeSize = ObjSize = { m_fCubesize.fX, m_fCubesize.fY, m_fCubesize.fZ };
                //이 녀석이 Build_OBJ에 해당될 경우
                // + 큐브Type 또는 PlaneType 라는 타입을 선택한 상황인 경우
                if ((CImGuiManager::GetInstance()->Get_OBJModeCheck() == true) &&
                    ((CImGuiManager::GetInstance()->Get_CubeType() == true) ||
                        (CImGuiManager::GetInstance()->Get_PlaneType() == true)))
                {
                    m_iTextureNum2 = CImGuiManager::GetInstance()->Get_OBJTexNum();
                    OBJ_TYPE eTypeTemp = CImGuiManager::GetInstance()->Get_OBJType(); //어떤 타입의 OBJ?
                    OBJ_ATTRIBUTE eAttribute = CImGuiManager::GetInstance()->Get_OBJATTRIBUTE(); //어떤 속성의 OBJ?
                    _uint RotateCount = CImGuiManager::GetInstance()->Get_OBJ_RotateCountCW();

                    pGameObject = CBuild_Obj::Create(m_pGraphicDev, CursorTemp, m_iTextureNum2, ObjSize, RotateCount, m_iOBJIndex, eTypeTemp, eAttribute);

                    NULL_CHECK_RETURN(pGameObject, E_FAIL);
                    FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_OBJ, pGameObject), E_FAIL);

                    m_mapLayer.insert({ LAYERTAG::ENVIRONMENT, m_pLayer });

                    OBJTemp->vSize = ObjSize;
                    OBJTemp->eOBJ_TYPE = eTypeTemp;
                    OBJTemp->eOBJ_Attribute = eAttribute;
                    OBJTemp->uiTextureNum = m_iTextureNum2;
                    OBJTemp->vPos = CursorTemp;
                    OBJTemp->iIndex = m_iOBJIndex;
                    OBJTemp->iRotateCount = RotateCount;
                    OBJTemp->uiOBJ_HP = dynamic_cast<CBuild_Obj*>(pGameObject)->Get_OBJ_HP();
                    OBJTemp->eOBJ_Interaction = dynamic_cast<CBuild_Obj*>(pGameObject)->Get_OBJ_Interaction();

                    if (eTypeTemp == OBJ_TYPE::CUBE_TYPE)
                    {
                        m_VecTempCube.clear();
                        m_VecTempCube = CImGuiManager::GetInstance()->Get_CubeTextureObjVector();
                        OBJTemp->pCubeTexture = m_VecTempCube[m_iTextureNum2 - cubeObjTextureStartIndex];
                    }
                    else if (eTypeTemp == OBJ_TYPE::PLANE_TYPE)
                    {
                        m_VecTempPlane.clear();
                        m_VecTempPlane = CImGuiManager::GetInstance()->Get_PlaneTextureObjVector();
                        OBJTemp->pBaseTexture = m_VecTempPlane[m_iTextureNum2 - planeObjTextureStartIndex];
                    }

                    if (eAttribute != OBJ_ATTRIBUTE::C_POINT_OBJ)
                    {
                        m_VecOBJData.push_back(OBJTemp);  // C_POINT 빼곤 다 담겨야함
                    }

                    if (eAttribute == OBJ_ATTRIBUTE::LIGHT_OBJ)
                    {
                        OBJTemp3 = new OBJData(*OBJTemp);
                        m_VecLight.push_back(OBJTemp3); //상동
                    }

                    if (eAttribute == OBJ_ATTRIBUTE::C_POINT_OBJ)
                    {
                        OBJ_C_POINT->defOBJData = *OBJTemp;
                        OBJ_C_POINT->eMonsterType = CImGuiManager::GetInstance()->Get_MonsterType();

                        m_VecCreatePoint.push_back(OBJ_C_POINT); //얜 이거 따로 저장
                    }

                    m_iOBJIndex++;
                }
            }

            if (m_Build_time_Check == true)
            {
                m_Build_time++;

                if (m_Build_time >= 7)
                {
                    m_Build_time = 0;
                    m_Build_time_Check = false;
                }
            }

            if (m_Build_time_Check2 == true)
            {
                m_Build_time2++;

                if (m_Build_time2 >= 7)
                {
                    m_Build_time2 = 0;
                    m_Build_time_Check2 = false;
                }
            }
        }
    }

    //
    // 	if (CImGuiManager::GetInstance()->Get_BuildModeCheck() == true) {
    // 		Safe_Delete(CubeTemp2);
    // 	}
    // 	else if (CImGuiManager::GetInstance()->Get_OBJModeCheck() == true) {
    //         Safe_Delete(OBJTemp);
    // 	}
    return S_OK;
}

/*
   Safe_Release ref 카운터가 올라가는데 이미 지웠는데 또 불렀거나,
   아직 카운터 올리지않았는데 불러
*/


HRESULT CMapTool::Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag)
{
	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile) { return E_FAIL; }

	DWORD   dwByte = 0;
	DWORD   dwStrByte = 0;
	CHAR* pTag = new CHAR[dwStrByte];


	if (eTag == OBJECTTAG::BUILD_CUBE) {
		string m_strText = "MapData";
		CUBE* pCube = nullptr;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
		m_strText = pTag;

		basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
		const _tchar* aa = converted.c_str();

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

		for (auto& iter : m_VecCubeData)
		{
			pGameObject = CBuild_Cube::Create(m_pGraphicDev, iter->vPos, iter->uITextureNum, iter->vSize, m_iCubeIndex);

			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_CUBE, pGameObject), E_FAIL);
			m_iCubeIndex++;

		}
		m_mapLayer.emplace(LAYERTAG::ENVIRONMENT, m_pLayer);
	}

    OBJData* LightTemp = nullptr;

	if (eTag == OBJECTTAG::BUILD_OBJ) {
		string m_strText = "OBJData";
		OBJData* pOBJ = nullptr;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
		m_strText = pTag;

		basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());

		while (true)
		{
			pOBJ = new OBJData;

			ReadFile(hFile, pOBJ, sizeof(OBJData), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pOBJ);
				break;
			}
			m_VecOBJData.push_back(pOBJ);
		}
		CloseHandle(hFile);

		Engine::CGameObject* pGameObject = nullptr;

		for (auto& iter : m_VecOBJData)
		{
			pGameObject = CBuild_Obj::Create(m_pGraphicDev, iter->vPos, iter->uiTextureNum, iter->vSize, iter->iRotateCount, m_iOBJIndex, iter->eOBJ_TYPE, iter->eOBJ_Attribute);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_OBJ, pGameObject), E_FAIL);

            if (iter->eOBJ_Attribute == OBJ_ATTRIBUTE::LIGHT_OBJ)
            {
                OBJData* LightTemp = new OBJData;
                LightTemp = iter;
                m_VecLight.push_back(LightTemp);
            }

			m_iOBJIndex++;
		}
		m_mapLayer.emplace(LAYERTAG::ENVIRONMENT, m_pLayer);
	}

	delete[] pTag;
	pTag = nullptr;

    delete LightTemp;
    LightTemp = nullptr;

	return S_OK;
}

HRESULT CMapTool::Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag)
{
	//파일 개방해서 받아오기
	string m_strText = "CPointData";

	HANDLE      hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD   dwByte = 0;
	DWORD   dwStrByte = 0;
	C_POINT* pOBJ = nullptr;

	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	CHAR* pTag = new CHAR[dwStrByte];

	ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
	m_strText = pTag;

	basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());

	//저장된 데이터대로 동적할당해서 벡터에 담기
	while (true)
	{
		pOBJ = new C_POINT;

		ReadFile(hFile, pOBJ, sizeof(C_POINT), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pOBJ);
			break;
		}
		m_VecCreatePoint.push_back(pOBJ);
	}
	CloseHandle(hFile);

	Engine::CGameObject* pGameObject = nullptr;

	//벡터 내용물만큼 실제 생성해 레이어에 담기
	for (auto& iter : m_VecCreatePoint)
	{
		pGameObject = CBuild_Obj::Create(m_pGraphicDev, iter->defOBJData.vPos, iter->defOBJData.uiTextureNum,
			iter->defOBJData.vSize, iter->defOBJData.iRotateCount, m_iOBJIndex, iter->defOBJData.eOBJ_TYPE, iter->defOBJData.eOBJ_Attribute);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_OBJ, pGameObject), E_FAIL);
		m_iOBJIndex++;
	}
	m_mapLayer.insert({ LAYERTAG::ENVIRONMENT, m_pLayer });


	delete[] pTag;
	pTag = nullptr;

	return S_OK;
}

HRESULT CMapTool::Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag)
{
	//파일 개방해서 받아오기
	string m_strText = "TriggerData";

	HANDLE      hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD   dwByte = 0;
	DWORD   dwStrByte = 0;
	TRIGGER* pTR = nullptr;

	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	char* pTag = new CHAR[dwStrByte];

	ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
	m_strText = pTag;

	basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());

	//저장된 데이터대로 동적할당해서 벡터에 담기
	while (true)
	{
		pTR = new TRIGGER;

		ReadFile(hFile, pTR, sizeof(TRIGGER), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTR);
			break;
		}

        m_TriggerData.push_back(pTR);
	}
	CloseHandle(hFile);

	Engine::CGameObject* pGameObject = nullptr;

	//벡터 내용물만큼 실제 생성해 레이어에 담기
	for (auto& iter : m_TriggerData)
	{
		pGameObject = CTrigger::Create(m_pGraphicDev, iter->vPos, iter->iIndex, iter->vSize, iter->eTrCase, iter->eTrType, iter->eTrName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pGLayer->Add_GameObject(OBJECTTAG::O_TRIGGER, pGameObject), E_FAIL);
	}
	m_mapLayer.emplace(LAYERTAG::GAMELOGIC, m_pGLayer);

	delete[] pTag;
	pTag = nullptr;

	return S_OK;
}


HRESULT CMapTool::Delete_Map()
{
    // 2개 벡터에서 지운다.
    // 1. 맵툴이 가지고 있는 create-delete & save-load 용 벡터
    // 2. Create 시 생성되어 Layer 가 갖게 되는 GameObject 벡터

    // 3개 더 추가
    // 3. (해당 시) 트리거 OBJ 벡터
    // 4. (해당 시) 무빙 OBJ 벡터
    // 5. (해당 시) CPoint OBJ 벡터

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

                    delete* iter;
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

        if (CImGuiManager::GetInstance()->Get_TriggerMode_Check() == true) {

            for (auto iter = m_TriggerData.begin(); iter != m_TriggerData.end();)
            {
                if (fabsf((*iter)->vPos.x - CursorTemp.x) < VTXITV &&
                    fabsf((*iter)->vPos.y - CursorTemp.y) < VTXITV &&
                    fabsf((*iter)->vPos.z - CursorTemp.z) < VTXITV)
                {
                    IndexTemp = (*iter)->iIndex;

                    delete* iter;
                    //*iter = nullptr;
                    iter = m_TriggerData.erase(iter);


                    if (IndexTemp != -1) {
                        auto& VectorTemp = Engine::Management()->GetInstance()->Get_Scene()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::O_TRIGGER);

                        for (int i = 0; i < VectorTemp.size(); ++i)
                        {
                            if (IndexTemp == VectorTemp[i]->Get_iIndex())
                            {
                                m_pLayer->Delete_GameObject(OBJECTTAG::O_TRIGGER, VectorTemp[i], IndexTemp);
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
            bool bOBJ_DeleteCheck = false;
            for (auto iter = m_VecOBJData.begin(); iter != m_VecOBJData.end();)
            {
                if (fabsf((*iter)->vPos.x - CursorTemp.x) < VTXITV &&
                    fabsf((*iter)->vPos.y - CursorTemp.y) < VTXITV &&
                    fabsf((*iter)->vPos.z - CursorTemp.z) < VTXITV)
                {
                    IndexTemp = (*iter)->iIndex;
                    bOBJ_DeleteCheck = true;

                    delete* iter;
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

            for (auto iter = m_VecCreatePoint.begin(); iter != m_VecCreatePoint.end();)
            {
                if (fabsf((*iter)->defOBJData.vPos.x - CursorTemp.x) < VTXITV &&
                    fabsf((*iter)->defOBJData.vPos.y - CursorTemp.y) < VTXITV &&
                    fabsf((*iter)->defOBJData.vPos.z - CursorTemp.z) < VTXITV)
                {
                    IndexTemp = (*iter)->defOBJData.iIndex;

                    delete* iter;
                    iter = m_VecCreatePoint.erase(iter);

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

                }
            }


            if (bOBJ_DeleteCheck == true)
            {
                if (!m_VecLight.empty())
                {
                    for (auto& iter = m_VecLight.begin(); iter != m_VecLight.end();)
                    {
                        if ((*iter)->iIndex == IndexTemp)
                        {
                            delete* iter;
                            iter = m_VecLight.erase(iter);
                            bOBJ_DeleteCheck = false;
                        }
                        else
                        {
                            ++iter;
                        }
                    }
                }
            }
        }
        int i = 0;
    }
    return S_OK;
}

bool CMapTool::CheckDuplicateCube(const _vec3& pPos, const _vec3& pSize)
{
    if (m_VecCubeData.empty())
        return false;

    bool bCheckPoint = true; //한 번 도는동안 계속 true라면(한번도 안 걸리면) 그게 곧 중복

    for (const auto& iter : m_VecCubeData)
    {
        if (bCheckPoint == false) // 중복이 안 발생했으니까 한 바퀴 다시 돌아야지
        {
            bCheckPoint = true;
        } //트루로 만들고 다시 돈다

        _vec3 minPoint1 = pPos - pSize * 0.5f;  //새로 비교하려고 하는 놈
        _vec3 maxPoint1 = pPos + pSize * 0.5f;

        _vec3 minPoint2 = iter->vPos - iter->vSize * 0.5f; //원래 설치된 놈
        _vec3 maxPoint2 = iter->vPos + iter->vSize * 0.5f;

        //정쌤 AABB 규칙에 따른 중복검사 루틴!!!! 여기에 걸리면 중복이 아닌거임

        float max_min = max(minPoint1.x, minPoint2.x);
        float min_max = min(maxPoint1.x, maxPoint2.x);
        bool result1 = max(minPoint1.x, minPoint2.x) >= min(maxPoint1.x, maxPoint2.x);

        if (max(minPoint1.x, minPoint2.x) >= min(maxPoint1.x, maxPoint2.x))
        {
            bCheckPoint = false; //x 라인 중복이 아니다
        }

        float max_min2 = max(minPoint1.y, minPoint2.y);
        float min_max2 = min(maxPoint1.y, maxPoint2.y);
        bool result2 = max(minPoint1.y, minPoint2.y) >= min(maxPoint1.y, maxPoint2.y);

        if (max(minPoint1.y, minPoint2.y) >= min(maxPoint1.y, maxPoint2.y))
        {
            bCheckPoint = false; //y 라인 중복이 아니다
        }

        float max_min3 = max(minPoint1.z, minPoint2.z);
        float min_max3 = min(maxPoint1.z, maxPoint2.z);
        bool result3 = max(minPoint1.z, minPoint2.z) >= min(maxPoint1.z, maxPoint2.z);

        if (max(minPoint1.z, minPoint2.z) >= min(maxPoint1.z, maxPoint2.z))
        {
            bCheckPoint = false; //z 라인 중복이 아니다
        }

        if (bCheckPoint == true) //트루로 시작해서 쭉 그대로 왔다는건 하나도 안 걸린 것
        {
            return true;
        }  // 즉 중복이 발생한 거임 그럼 바로 return한다
    }

    return false; // 안 겹쳤어 끝까지! = 설치 가능/ 중복 아님
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
    Safe_Delete(OBJTemp2);
    Safe_Delete(OBJTemp3);
    Safe_Delete(CubeTemp2);
    Safe_Delete(OBJ_C_POINT);

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
    //m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

    __super::Free();
}