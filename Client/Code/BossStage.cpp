#include "stdafx.h"
#include "BossStage.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "EventMgr.h"
#include "ImGuiManager.h"
#include "EventMgr.h"
#include "DustGrey.h"
#include <sstream>
#include <utility>
#include "LoadingStage1.h"
#include "MonsterBombEffect.h"
#include "MBulletBombEffect.h"
#include "MBulletExplosion.h"


#include "FootRay.h"
CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(LAYERTAG::CAMERA), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	Load_Data_C(L"../Bin/Data/CPoint/BossStage/CPointData", OBJECTTAG::BUILD_OBJ); //TODO

	srand(GetTickCount64());

	//TODO - 승용추가 크로스헤어 추가, 기본 커서 안보이게
	ShowCursor(FALSE);

	SoundMgr()->PlayBGM(L"BossBGM.wav",0.5f);
	m_eSceneTag = SCENETAG::BOSS_STAGE;
	return S_OK;
}

_int CBossStage::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);
 	if (m_bLateInit)
 	{
 		//CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::BOSS_STAGE, DIALOGTAG::ST1_BOSS_START);
 		//CEventMgr::GetInstance()->OnPause(true, SCENETAG::BOSS_STAGE);
 		m_bLateInit = false;
 	}

	if (!Management()->Get_Scene()->Get_Pause())
	{
		m_pBoss->Set_Start(true);
		m_pMission->Set_Render(false);

		CGameObject* pGameObject = nullptr;
		CLayer* pLayer = nullptr;

		pLayer = Management()->Get_Layer(LAYERTAG::UI);

		if (m_bOneCreate)
		{
			pGameObject = CBossHPFrame::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

			pGameObject = CBossHPValue::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

			m_bOneCreate = false;
		}

	}

	if(m_pBoss->Get_Info().bDead)
		m_pMission->Set_Render(true);
	//if (m_bReadyCube)
	//{
	//	Octree()->Update_Octree();
	//}

	m_fAdminTick += fTimeDelta;
	if (m_fAdminTick >= 3.f)
	{
		m_bAdminSwitch = true;
		m_fAdminTick = 0.F;
	}

	Admin_KeyInput();
	return iExit;
}

void CBossStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();


	CollisionManager()->LateUpdate_Collision();
}

void CBossStage::Render_Scene()
{
}

HRESULT CBossStage::Create_Monster()
{
	if (m_bCreateMonster == false)
	{
		Engine::CGameObject* pGameObject = nullptr;

		for (auto& iter : m_VecCreatePoint)
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
		m_bCreateMonster = true;
	}
	return S_OK;
}

HRESULT CBossStage::Ready_LightInfo()
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

HRESULT CBossStage::Ready_Prototype()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UITex", CUITex::Create(m_pGraphicDev)), E_FAIL);
	return S_OK;
}

HRESULT CBossStage::Ready_Layer_Environment(LAYERTAG eLayerTag)
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

	Load_Data(L"../Bin/Data/Map/BossStage/MapData", OBJECTTAG::BUILD_CUBE);
	Load_Data(L"../Bin/Data/OBJ/BossStage/OBJData", OBJECTTAG::BUILD_OBJ);

	m_mapLayer.insert({ eLayerTag, m_pLayer });

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_GameLogic(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = m_pGLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	{
		// Player
		CGameObject* pPlayer = pGameObject = Management()->Get_Player();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER, pGameObject), E_FAIL);	//플레이어

		dynamic_cast<CPlayer*>(pPlayer)->ClearGunList();


		pGameObject = Management()->Get_ShotGun();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);


		pGameObject = Management()->Get_Rifle();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);

		pGameObject = Management()->Get_RifleHand();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_HAND, pGameObject), E_FAIL);

		dynamic_cast<CPlayer*>(pPlayer)->Set_SceneChange(false);
		dynamic_cast<CPlayer*>(pPlayer)->SetGun(pLayer);
		pPlayer->Get_Transform()->Set_Pos(92.5f,10.0f,10.0f);



		//FootRay
		pGameObject = CFootRay::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);
		dynamic_cast<CFootRay*>(pGameObject)->Set_Host(pPlayer);

		//
		//pGameObject = CRay::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);


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

		pGameObject = CStage1Boss::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BOSS, pGameObject), E_FAIL);

		m_pBoss = dynamic_cast<CStage1Boss*>(pGameObject);

	}


	//이펙트 파티클
	{
		pGameObject = CMonsterBombEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);

		pGameObject = CMBulletBombEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);

		//파티클
		pGameObject = CDustGrey::Create(m_pGraphicDev, _vec3(0.f, 0.f, 0.f), 256);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::PARTICLE, pGameObject), E_FAIL);

		pGameObject = CMBulletExplosion::Create(m_pGraphicDev, _vec3(0.f, 0.f, 0.f), 256);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::PARTICLE, pGameObject), E_FAIL);
	}
	// 보스

	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_Camera(LAYERTAG eLayerTag)
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

HRESULT CBossStage::Ready_Layer_UI(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CCrossHair::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::CROSSHAIR, pGameObject), E_FAIL);

	m_pMission = CMissionObjective::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMission, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::MISSION, m_pMission), E_FAIL);

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

	//승용

	//Load_UI();

	//pGameObject = CImGuiManager::GetInstance()->Get_UI(L"Checkmark.png");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	//pGameObject = CUIMgr::GetInstance()->Get_UI(L"electricity-icon.png");
	//pGameObject = CImGuiManager::GetInstance()->Get_ImguiUI();




	return S_OK;
}

HRESULT CBossStage::Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag)
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


HRESULT CBossStage::Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag)
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

HRESULT CBossStage::Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag)
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


void CBossStage::Admin_KeyInput()
{

	if (Engine::Get_DIKeyState(DIK_F9) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnPause(false, SCENETAG::BOSS_STAGE);
		m_bAdminSwitch = false;
	}


	if (Engine::Get_DIKeyState(DIK_M) & 0x80 && m_bAdminSwitch)
	{
		CLoadingStage1* pScene = nullptr;
		pScene = CLoadingStage1::Create(m_pGraphicDev, SCENETAG::STAGE2);

		//CUIMgr::GetInstance()->DestroyInstance();
		Management()->Get_Player()->Set_SceneChange(true);
		Management()->Set_SYSceneChange(true);
		Management()->Change_Scene(pScene);


		m_bAdminSwitch = false;
	}
}

CBossStage* CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossStage* pInstance = new CBossStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossStage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossStage::Free()
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

	//for (int i = 0; i < m_VecCreatePoint.size(); ++i)
	//{
	//	Safe_Delete(m_VecCreatePoint[i]);
	//}
	//m_VecCreatePoint.clear();



	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	__super::Free();
}
