#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "ImGuiManager.h"
#include "EventMgr.h"
#include "DustGrey.h"
#include <sstream>
#include <utility>
#include "MonsterBombEffect.h"
#include "MBulletBombEffect.h"
#include "MBulletExplosion.h"
#include "SoundMgr.h"

#include "FootRay.h"

#include "LoadingStage1.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	Engine::CGameObject* pGameObject = nullptr;
}

CStage::~CStage()
{
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

	//Load_Data_T(L"../Bin/Data/Trigger/Stage1/TriggerData", OBJECTTAG::O_TRIGGER);
	Load_Data_C(L"../Bin/Data/CPoint/Stage1/C0/CPointData", OBJECTTAG::BUILD_CUBE, 0);

	//TODO - 승용추가 크로스헤어 추가, 기본 커서 안보이게
	ShowCursor(FALSE);

	CSoundMgr::GetInstance()->PlayBGM(L"MainBGM1.wav", 0.15);

	return S_OK;
}



_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	if (m_bLateInit)
	{
		//CEventMgr::GetInstance()->OnDialog(m_pGraphicDev,m_eSceneTag, DIALOGTAG::STORY_ST1_INTRO);
		//CEventMgr::GetInstance()->OnPause(true,m_eSceneTag);
		m_bLateInit = false;
	}

// 	if (m_bReadyCube)
// 	{
// 		Octree()->Update_Octree();
// 	}

	m_fAdminTick += fTimeDelta;
	if (m_fAdminTick >= 3.f)
	{
		m_bAdminSwitch = true;
		m_fAdminTick = 0.F;
	}


	return iExit;
}

void CStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
	CollisionManager()->LateUpdate_Collision();

	Admin_KeyInput();

	Trigger_Check_For_Create_Monster();

}

void CStage::Render_Scene()
{
	if (m_bReadyCube)
	{
		Octree()->Render_Octree(m_pGraphicDev);
	}
	//Debug

}

HRESULT CStage::Ready_LightInfo()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//mainApp 말고 stage-Maptool 단에서 켜기
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

	Load_Data(L"../Bin/Data/Map/MapData", OBJECTTAG::BUILD_CUBE);		// 큐브텍스
	Load_Data(L"../Bin/Data/OBJ/OBJData", OBJECTTAG::BUILD_OBJ);		//TODO 얘는 섞여있다 큐브텍스랑 rcTEXT
		
	m_mapLayer.insert({ eLayerTag, m_pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = m_pGLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	{
		// Player
		CGameObject* pPlayer = m_pPlayer = pGameObject = Management()->Get_Player();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER, pGameObject), E_FAIL);	//플레이어

		pGameObject = Management()->Get_ShotGun();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);


		pGameObject = Management()->Get_Rifle();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);


		pGameObject = Management()->Get_RifleHand();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_HAND, pGameObject), E_FAIL);

		pGameObject = Management()->Get_ShotGunFlash();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);	//샷건섬광

		pGameObject = Management()->Get_RifleFlash();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);	//라이플 섬광

		pGameObject = Management()->Get_Lazer();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_LAZER, pGameObject), E_FAIL);



		pGameObject = Management()->Get_Belt();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::OBJECT, pGameObject), E_FAIL);	//벨트
		// 벨트임

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
	for (auto& iter : m_VecCreatePoint[0])
	{
		if (iter->eMonsterType == MonsterType::BIGDADDY)
		{
			pGameObject = CBigDaddyMonster::Create(m_pGraphicDev, iter->defOBJData.vPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
		}
		if (iter->eMonsterType == MonsterType::DULLSUIT)
		{
			pGameObject = CDullSuitMonster::Create(m_pGraphicDev, iter->defOBJData.vPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
		}
		if (iter->eMonsterType == MonsterType::KCIKBOARD)
		{
			pGameObject = CKickBoardMonster::Create(m_pGraphicDev, iter->defOBJData.vPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
		}
	}

	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Camera(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = Management()->Get_Camera();
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

	pGameObject = CHPBarFrame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CHPBarValue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CHPMark::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CShieldFrame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CShieldValue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CShieldMark::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CBerserkFrame_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CBerserk_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CPlayerFace::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CHat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CBulletInfoName::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CBulletInfoCount::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CWeaponInfo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	pGameObject = CRopeUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);


	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Trigger_Check_For_Create_Monster()
{
	_bool CheckTemp = dynamic_cast<CPlayer*>(m_pPlayer)->Get_TriggerCheck();
	
	TRIGGER_NUMBER	m_eTrName = TRIGGER_NUMBER::TR_END;
	TRIGGER_STATE	m_eTrState = TRIGGER_STATE::TR_STATE_END;

	if (CheckTemp == true)
	{
		m_eTrName = dynamic_cast<CPlayer*>(m_pPlayer)->Get_TR_NUMBER();
		m_eTrState = dynamic_cast<CPlayer*>(m_pPlayer)->Get_TR_STATE();
	}

	if ((m_eTrName != TRIGGER_NUMBER::TR_END) && (m_eTrState != TRIGGER_STATE::TR_STATE_END))
	{
		dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_STATE(TRIGGER_STATE::TR_NOW);
		int iCountNum = -1;

		if (m_eTrName == TRIGGER_NUMBER::TR0)
		{
			iCountNum = 1;
			Load_Data_C(L"../Bin/Data/CPoint/Stage1/C1/CPointData", OBJECTTAG::BUILD_OBJ, iCountNum);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_STATE(TRIGGER_STATE::TR_AFTER);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_NUMBER(TRIGGER_NUMBER::TR_END);
		}	
		if (m_eTrName == TRIGGER_NUMBER::TR1)
		{
			iCountNum = 2;
			Load_Data_C(L"../Bin/Data/CPoint/Stage1/C2/CPointData", OBJECTTAG::BUILD_OBJ, iCountNum);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_STATE(TRIGGER_STATE::TR_AFTER);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_NUMBER(TRIGGER_NUMBER::TR_END);
		}
		if (m_eTrName == TRIGGER_NUMBER::TR2)
		{
			iCountNum = 3;
			Load_Data_C(L"../Bin/Data/CPoint/Stage1/C3/CPointData", OBJECTTAG::BUILD_OBJ, iCountNum);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_STATE(TRIGGER_STATE::TR_AFTER);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_TR_NUMBER(TRIGGER_NUMBER::TR_END);
		}

		Engine::CGameObject* pGameObject = nullptr;

		for (auto& iter : m_VecCreatePoint[iCountNum])
		{
			if (iter->eMonsterType == MonsterType::BIGDADDY)
			{
				pGameObject = CBigDaddyMonster::Create(m_pGraphicDev, iter->defOBJData.vPos);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(m_pGLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
			}
			if (iter->eMonsterType == MonsterType::DULLSUIT)
			{
				pGameObject = CDullSuitMonster::Create(m_pGraphicDev, iter->defOBJData.vPos);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(m_pGLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
			}
			if (iter->eMonsterType == MonsterType::KCIKBOARD)
			{
				pGameObject = CKickBoardMonster::Create(m_pGraphicDev, iter->defOBJData.vPos);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(m_pGLayer->Add_GameObject(OBJECTTAG::MONSTER, pGameObject), E_FAIL);
			}
		}
	}

	return S_OK;
}

HRESULT CStage::Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag)
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

HRESULT CStage::Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag, int CountNum)
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
		m_VecCreatePoint[CountNum].push_back(pOBJ);
	}
	CloseHandle(hFile);

	Engine::CGameObject* pGameObject = nullptr;

	//벡터 내용물만큼 실제 생성해 레이어에 담기
	for (auto& iter : m_VecCreatePoint[CountNum])
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

HRESULT CStage::Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag)
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

		m_TriggerDataTemp.push_back(pTR);
	}
	CloseHandle(hFile);

	Engine::CGameObject* pGameObject = nullptr;

	//벡터 내용물만큼 실제 생성해 레이어에 담기
	for (auto& iter : m_TriggerDataTemp)
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


void CStage::Admin_KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_F4) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnLevelUp(m_pGraphicDev, SCENETAG::STAGE);
		CEventMgr::GetInstance()->OnPause(true,SCENETAG::STAGE);
		m_bAdminSwitch = false;
	}
	if (Engine::Get_DIKeyState(DIK_F9) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnPause(false, SCENETAG::STAGE);
		m_bAdminSwitch = false;
	}

	if (Engine::Get_DIKeyState(DIK_F8) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::STAGE, DIALOGTAG::STORY_ST1_INTRO);
		m_bAdminSwitch = false;
	}

	if (Engine::Get_DIKeyState(DIK_M) & 0x80 && m_bAdminSwitch)
	{
		CLoadingStage1* pScene = nullptr;
		pScene = CLoadingStage1::Create(m_pGraphicDev, SCENETAG::LOBBY);

		//CUIMgr::GetInstance()->DestroyInstance();
		Management()->Get_Player()->Set_SceneChange(true);
		Management()->Set_SYSceneChange(true);
		Management()->Change_Scene(pScene);


		m_bAdminSwitch = false;
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
	SoundMgr()->StopAll();

	for (int i = 0; i < m_VecCubeData.size(); ++i)
	{
		Safe_Delete(m_VecCubeData[i]);
	}
	m_VecCubeData.clear();

	for (int i = 0; i < m_VecOBJData.size(); ++i)
	{
		Safe_Delete(m_VecOBJData[i]);
	}
	m_VecOBJData.clear();

	int CountNum = 4;
	for (int i = 0; i < CountNum; ++i) {
		for (int j = 0; j < m_VecCreatePoint[CountNum].size(); ++j)
		{
			Safe_Delete(m_VecCreatePoint[CountNum][j]);
		}
		m_VecCreatePoint[CountNum].clear();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	__super::Free();
}
