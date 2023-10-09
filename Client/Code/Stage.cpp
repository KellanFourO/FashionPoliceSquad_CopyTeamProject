#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "ImGuiManager.h"
#include "UIMgr.h"
#include "EventMgr.h"
#include "DustGrey.h"
#include <sstream>
#include <utility>

#include "FootRay.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
	Free();
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(LAYERTAG::CAMERA), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	

	srand(GetTickCount64());

	//TODO - 승용추가 크로스헤어 추가, 기본 커서 안보이게
	ShowCursor(FALSE);

	return S_OK;
}



_int CStage::Update_Scene(const _float& fTimeDelta)
{

	_int	iExit = __super::Update_Scene(fTimeDelta);

	if (m_bReadyCube)
	{
		Octree()->Update_Octree();
	}

	m_fAdminTick += fTimeDelta;
	if (m_fAdminTick >= 3.f)
	{
		m_bAdminSwitch = true;
		m_fAdminTick = 0.F;
	}

	Admin_KeyInput();

	return iExit;
}

void CStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();


	CollisionManager()->LateUpdate_Collision();



	Engine::CGameObject* pGameObject = nullptr;



}

void CStage::Render_Scene()
{
	//if (m_bReadyCube)
	//{
	//	Octree()->Render_Octree(m_pGraphicDev);
	//}
	//Debug

}

HRESULT CStage::Ready_LightInfo()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//mainApp 말고 stage-Maptool 단에서 켜기
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

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

HRESULT CStage::Ready_Prototype()
{

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environment(LAYERTAG eLayerTag)
{
	m_pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;
	Engine::CGameObject* pTemp = nullptr;

	pTemp = pGameObject;

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::SKYBOX, pGameObject), E_FAIL);

	pTemp = pGameObject;

	

	Load_Data(L"../Bin/Data/Map/MapData", OBJECTTAG::BUILD_CUBE);
	Load_Data(L"../Bin/Data/OBJ/OBJData", OBJECTTAG::BUILD_OBJ);

	m_mapLayer.insert({ eLayerTag, m_pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	{
		// Player
		CGameObject* pPlayer = pGameObject = CPlayer::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER, pGameObject), E_FAIL);	//플레이어

		pGameObject = CPaintShotGun::Create(m_pGraphicDev,dynamic_cast<CPlayer*>(pPlayer));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);
		dynamic_cast<CPaintShotGun*>(pGameObject)->Set_Host(pPlayer);
		
		pGameObject = CTailorAssertRifle::Create(m_pGraphicDev, dynamic_cast<CPlayer*>(pPlayer));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);
		dynamic_cast<CTailorAssertRifle*>(pGameObject)->Set_Host(pPlayer);

		pGameObject = CTailorAssertRifleHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_HAND, pGameObject), E_FAIL);

		//FootRay
		pGameObject = CFootRay::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);
		dynamic_cast<CFootRay*>(pGameObject)->Set_Host(pPlayer);

		pGameObject = CMuzzleFlash::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);	//샷건섬광

		pGameObject = CMuzzleFlash_Rifle::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);	//라이플 섬광

		pGameObject = CLazer::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_LAZER, pGameObject), E_FAIL);



		pGameObject = CBelt::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::OBJECT, pGameObject), E_FAIL);	//벨트
		// 벨트임

		//pGameObject = CInventory::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::ITEM, pGameObject), E_FAIL);
		//dynamic_cast<CInventory*>(pGameObject)->Set_Player(dynamic_cast<CPlayer*>(pPlayer));	//인벤토리



	}
	{
		//for (int i = 0; i < 6; i++) {
		//	pGameObject = CItem::Create(m_pGraphicDev);
		//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//	Item_Info Item_Info;
		//	Item_Info.Item_ID = (ItemID)i;
		//	Item_Info.vPos = _vec3(20 + 6 * i, 2.f, 35);
		//	dynamic_cast<CItem*>(pGameObject)->Set_INFO(Item_Info);
		//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::ITEM, pGameObject), E_FAIL);	//아이템 배치
		//}
	}
	// 아이템


	//몬스터

	{
		pGameObject = CStage1Boss::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BOSS, pGameObject), E_FAIL);
	}
	for(int i = 0; i<3; ++i)
	{
	pGameObject = CBigDaddyMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);

	pGameObject = CKickBoardMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);

	pGameObject = CDullSuitMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
	
	}

//이펙트 파티클
	{
		//이펙트
		pGameObject = CEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);
		//파티클
		pGameObject = CDustGrey::Create(m_pGraphicDev, _vec3(50.f, 10.f, 25.f), 128);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::PARTICLE, pGameObject), E_FAIL);
	}
	
	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Camera(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	// FPSCamera
// 	pGameObject = CFPSCamera::Create(m_pGraphicDev,
// 		&_vec3(0.f, 10.f, -10.f),
// 		&_vec3(0.f, 0.f, 1.f),
// 		&_vec3(0.f, 1.f, 0.f));

	pGameObject = CNewFPSCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::FPSCAMERA, pGameObject), E_FAIL);



	m_mapLayer.insert({ eLayerTag, pLayer });
	//CImGuiManager::GetInstance()->Set_Cam(dynamic_cast<CDynamicCamera*>(pGameObject));
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CCrossHair::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::CROSSHAIR, pGameObject), E_FAIL);

	pGameObject = CMissionObjective::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MISSION, pGameObject), E_FAIL);

	
	
	
	

	m_mapLayer.insert({ eLayerTag, pLayer });

	//승용
	Load_UI();

	//pGameObject = CImGuiManager::GetInstance()->Get_UI(L"Checkmark.png");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	//pGameObject = CUIMgr::GetInstance()->Get_UI(L"electricity-icon.png");
	//pGameObject = CImGuiManager::GetInstance()->Get_ImguiUI();




	return S_OK;
}

HRESULT CStage::Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag)
{
	if (eTag == OBJECTTAG::BUILD_CUBE) {
		//파일 개방해서 받아오기
		string m_strText = "MapData";
		HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		DWORD   dwByte = 0;
		DWORD   dwStrByte = 0;
		CUBE* pCube = nullptr;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		CHAR* pTag = new CHAR[dwStrByte];

		ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
		m_strText = pTag;

		basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());
		const _tchar* aa = converted.c_str();

		//저장된 데이터대로 큐브 동적할당해서 벡터에 담기
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

		//벡터 내용물만큼 실제 큐브 생성해 레이어에 담기
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
	}

	if (eTag == OBJECTTAG::BUILD_OBJ) {
		//파일 개방해서 받아오기
		string m_strText = "MapData";

		HANDLE      hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		DWORD   dwByte = 0;
		DWORD   dwStrByte = 0;
		OBJData* pOBJ = nullptr;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		CHAR* pTag = new CHAR[dwStrByte];

		ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
		m_strText = pTag;

		basic_string<TCHAR> converted(m_strText.begin(), m_strText.end());

		//저장된 데이터대로 OBJ 동적할당해서 벡터에 담기
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

		//벡터 내용물만큼 실제 OBJ 생성해 레이어에 담기
		for (auto& iter : m_VecOBJData)
		{
			pGameObject = CBuild_Obj::Create(m_pGraphicDev, iter->vPos, iter->uiTextureNum, iter->vSize, iter->iRotateCount, m_iOBJIndex, iter->eOBJ_TYPE, iter->eOBJ_Attribute);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::BUILD_OBJ, pGameObject), E_FAIL);
			m_iOBJIndex++;
		}
		m_mapLayer.insert({ LAYERTAG::ENVIRONMENT, m_pLayer });

		delete[] pTag;
		pTag = nullptr;
	}

	return S_OK;
}


HRESULT CStage::Load_UI()
{
	Engine::CGameObject* pGameObject = nullptr;

	wifstream fin;

	fin.open(L"../Bin/Data/UI/UIdata.dat");

	if (!fin.fail())
	{
		wstring line;

		while (getline(fin, line)) // 한 줄씩 읽기
		{
			wistringstream iss(line);
			wstring token;

			_tchar szKey[MAX_PATH] = L"";
			_tchar szSizeX[MAX_PATH] = L"";
			_tchar szSizeY[MAX_PATH] = L"";
			_tchar szSizeZ[MAX_PATH] = L"";
			_tchar szPosX[MAX_PATH] = L"";
			_tchar szPosY[MAX_PATH] = L"";
			_tchar szPosZ[MAX_PATH] = L"";

			getline(iss, token, L','); // ','를 구분자로 사용하여 값을 자름
			_tcscpy_s(szKey, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szSizeX, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szSizeY, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szSizeZ, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szPosX, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szPosY, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szPosZ, MAX_PATH, token.c_str());

			if (wcsstr(szKey, L"des"))
			{
				continue;
			}
			pGameObject = CUIMgr::GetInstance()->Get_UI(szKey);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

		}
	}
	fin.close();

		m_mapLayer.insert({ LAYERTAG::UI, m_pLayer });

	return S_OK;
}

void CStage::Admin_KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_F9) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnLevelUp(m_pGraphicDev, SCENETAG::STAGE);
		m_bAdminSwitch = false;
	}

	if (Engine::Get_DIKeyState(DIK_F8) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::STAGE, DIALOGTAG::STORY_ST1_INTRO);
		m_bAdminSwitch = false;
	}

	if (Engine::Get_DIKeyState(DIK_F9) & 0x80)
	{
		CEventMgr::GetInstance()->OnPause(true);

	}

	if (Engine::Get_DIKeyState(DIK_F10) & 0x80)
	{
		CEventMgr::GetInstance()->OnPause(false);

	}
}


CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage* pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStage::Free()
{
 
// 	for (int i = 0; i < m_VecTempCube.size(); ++i)
// 	{
// 		Safe_Delete(m_VecTempCube[i]);
// 	}
// 	m_VecTempCube.clear();
// 
// 
// 	for (int i = 0; i < m_VecTempPlane.size(); ++i)
// 	{
// 		Safe_Delete(m_VecTempPlane[i]);
// 	}
// 	m_VecTempPlane.clear();


	for (int i = 0; i < m_VecCubeData.size(); ++i)
	{
		Safe_Delete(m_VecCubeData[i]);
	}
	m_VecCubeData.clear();

	__super::Free();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
