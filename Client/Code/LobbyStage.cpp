#include "stdafx.h"
#include "LobbyStage.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "EventMgr.h"
#include "ImGuiManager.h"
#include "DustGrey.h"
#include <sstream>
#include <utility>
#include "LoadingStage1.h"

#include "FootRay.h"
CLobbyStage::CLobbyStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

CLobbyStage::~CLobbyStage()
{
}

HRESULT CLobbyStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(LAYERTAG::CAMERA), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_MINIGAME(LAYERTAG::MINIGAME), E_FAIL);

	Load_Data_T(L"../Bin/Data/Trigger/Lobby/TriggerData", OBJECTTAG::O_TRIGGER); //TODO 트리거

	Add_Light();

	srand(GetTickCount64());

	//TODO - 승용추가 크로스헤어 추가, 기본 커서 안보이게
	ShowCursor(FALSE);

	m_eSceneTag = SCENETAG::LOBBY;


	//SoundMgr()->PlayBGM(L"LobbyBGM1.mp3",0.25f);	//Good
	//SoundMgr()->PlayBGM(L"LobbyBGM2.mp3", 0.25f);	//Good
	SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 0.25f);	//Good
	//SoundMgr()->PlayBGM(L"LobbyBGM4.mp3", 0.25f); //

	return S_OK;
}

_int CLobbyStage::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);
	//if (m_bLateInit)
	//{
	//	CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::LOBBY, DIALOGTAG::ST1_BOSS_START);
	//	CEventMgr::GetInstance()->OnPause(true, SCENETAG::LOBBY);
	//	m_bLateInit = false;
	//}

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



	Light_OnOff_Check();

	Admin_KeyInput();
	return iExit;
}

void CLobbyStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();


	CollisionManager()->LateUpdate_Collision();

	Moving_Wall();
}

void CLobbyStage::Render_Scene()
{
}

HRESULT CLobbyStage::Ready_LightInfo()
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


HRESULT CLobbyStage::Add_Light()
{
	if (!m_VecLight.empty()) {
		const int LightNum = m_VecLight.size();
		D3DLIGHT9* tLightInfoo = new D3DLIGHT9[LightNum];

		for (int i = 0; i != LightNum; ++i)
		{
			ZeroMemory(&tLightInfoo[i], sizeof(D3DLIGHT9));

			tLightInfoo[i].Type = D3DLIGHT_POINT;

			tLightInfoo[i].Diffuse = { 1.f, 1.f, 1.f, 0.4f };     // 색깔(난반사)
			tLightInfoo[i].Specular = { 1.f, 1.f, 1.f, 0.4f };    // 정반사
			tLightInfoo[i].Ambient = { 1.f, 1.f, 1.f, 0.4f };    // 환경반사

			_vec3 Height = { 0.f, 0.f, 0.f };
			_float Distance = 20.f;

			tLightInfoo[i].Position = (m_VecLight[i]->vPos) + Height;
			tLightInfoo[i].Range = Distance;
			tLightInfoo[i].Falloff = 1;  //거리 감쇄 - 선형 감소

			FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfoo[i], i + 1), E_FAIL);
			// 0은 전역(방향성)조명이라서 없애버리면 안 됨
		}

		delete[] tLightInfoo;
		tLightInfoo = nullptr;
	}

	return S_OK;
}

HRESULT CLobbyStage::Light_OnOff_Check()
{
	const _float stdDistance = 300.f; // 기준 거리
	const _float stdFov = D3DXToRadian(60.f); // 기준 시야각

	_vec3 vPlayerPos, vPlayerLook, vtoLight;
	m_pPlayer->Get_Transform()->Get_Info(INFO_POS, &vPlayerPos);
	m_pPlayer->Get_Transform()->Get_Info(INFO_LOOK, &vPlayerLook);

	_float LightDistance = 0.f;

	vector<OBJData*> SortLight;

	for (int i = 0; i != m_VecLight.size(); ++i)
	{
		vtoLight = m_VecLight[i]->vPos - vPlayerPos;
		LightDistance = D3DXVec3Length(&vtoLight);

		if (LightDistance <= stdDistance) { //거리가 기준보다 멀지 않고

			vtoLight = m_VecLight[i]->vPos - vPlayerPos;
			D3DXVec3Normalize(&vtoLight, &vtoLight);

			_float DotResult = D3DXVec3Dot(&vPlayerLook, &vtoLight);
			_float fAngle = D3DXToRadian(acos(DotResult));

			if (!(fAngle < stdFov)) //시야각 안에 있으면
			{
				SortLight.push_back(m_VecLight[i]);
			}
		}
	}

	while (SortLight.size() > 8) {

		_float maxDistance = -1.f;
		int maxIndex = -1;

		for (int i = 0; i < SortLight.size(); ++i) {

			_vec3 vtoLight = SortLight[i]->vPos - vPlayerPos;
			_float distance = D3DXVec3Length(&vtoLight);

			if (distance > maxDistance) {

				maxDistance = distance;
				maxIndex = i;
			}
		}
		if (maxIndex != -1) {
			SortLight.erase(SortLight.begin() + maxIndex);
		}
	}

	if (SortLight.size() <= 8)
	{
		for (int i = 0; i < m_VecLight.size(); ++i) {

			bool lightEnabled = false;

			for (int j = 0; j < SortLight.size(); ++j) {

				if (m_VecLight[i]->iIndex == SortLight[j]->iIndex) {
					lightEnabled = true;
					break;
				}
			}
			m_pGraphicDev->LightEnable(i + 1, lightEnabled);
		}
	}
	m_pGraphicDev->LightEnable(0, TRUE); // 전역은 무조건 활성화

	return S_OK;
}

HRESULT CLobbyStage::Moving_Wall()
{
	int iCheckTemp = -1;

	if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == true) ||
		(CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == true) ||
		(CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == true))
	{

		if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == false)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == false))
		{
			iCheckTemp = 0;
		}

		if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == false))
		{
			iCheckTemp = 1;
		}

		if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == true))
		{
			iCheckTemp = 2;
		}

		vector<CGameObject*> tempOBJList = Management()->Get_One_Scene(SCENETAG::LOBBY)->Get_Layer(LAYERTAG::ENVIRONMENT)->Get_ObjectList(OBJECTTAG::BUILD_OBJ);

		for (auto& iter : tempOBJList)
		{
			CBuild_Obj* BuildOBJTemp = dynamic_cast<CBuild_Obj*>(iter);
			_vec3 OBJPos = BuildOBJTemp->Get_myOBJPos();

			if ((BuildOBJTemp->Get_OBJ_ATTRIBUTE() == OBJ_ATTRIBUTE::STD_OBJ)
				&& ((iter->m_iIndex == (m_VecMoving[0]->iIndex - 10000)) ||
					(iter->m_iIndex == (m_VecMoving[1]->iIndex - 10000)))
				&& iCheckTemp == 0)
			{
				if (m_VecMoving[0]->vPos.y > -50)
				{
					m_VecMoving[0]->vPos.y -= 1.f;
					BuildOBJTemp->Get_TransformCom()->Set_Pos(m_VecMoving[0]->vPos);
				}
				if (m_VecMoving[1]->vPos.y > -50)
				{
					m_VecMoving[1]->vPos.y -= 1.f;
					BuildOBJTemp->Get_TransformCom()->Set_Pos(m_VecMoving[1]->vPos);
				}
			}

			{ // 두 번째 문
				if ((BuildOBJTemp->Get_OBJ_ATTRIBUTE() == OBJ_ATTRIBUTE::STD_OBJ) &&
					(OBJPos.y == m_VecMoving[2]->vPos.y) && (MovingCheck == false))
				{	//한 번만 수행
					MovingIndextemp = BuildOBJTemp->m_iIndex;
					MovingCheck = true;
				}

				if ((BuildOBJTemp->Get_OBJ_ATTRIBUTE() == OBJ_ATTRIBUTE::STD_OBJ)
					&& (BuildOBJTemp->m_iIndex == MovingIndextemp) && iCheckTemp == 1)
				{
					if (m_VecMoving[2]->vPos.y > -50)
					{
						m_VecMoving[2]->vPos.y -= 1.f;
						BuildOBJTemp->Get_TransformCom()->Set_Pos(m_VecMoving[2]->vPos);
					}
				}
			}

			{ //세 번째 문
				if ((BuildOBJTemp->Get_OBJ_ATTRIBUTE() == OBJ_ATTRIBUTE::STD_OBJ) &&
					(OBJPos.y == m_VecMoving[3]->vPos.y) && (MovingCheck2 == false))
				{	//한 번만 수행
					MovingIndextemp2 = BuildOBJTemp->m_iIndex;
					MovingCheck2 = true;
				}

				if ((BuildOBJTemp->Get_OBJ_ATTRIBUTE() == OBJ_ATTRIBUTE::STD_OBJ)
					&& (BuildOBJTemp->m_iIndex == MovingIndextemp2) && iCheckTemp == 2)
				{
					if (m_VecMoving[3]->vPos.y < 45)
					{
						m_VecMoving[3]->vPos.y += 1.f;
						BuildOBJTemp->Get_TransformCom()->Set_Pos(m_VecMoving[3]->vPos);
					}
				}
			}
		}
	}
	return S_OK;
}


HRESULT CLobbyStage::Ready_Prototype()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UITex", CUITex::Create(m_pGraphicDev)), E_FAIL);
	return S_OK;
}

HRESULT CLobbyStage::Ready_Layer_Environment(LAYERTAG eLayerTag)
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



	Load_Data(L"../Bin/Data/Map/Lobby/MapData", OBJECTTAG::BUILD_CUBE);
	Load_Data(L"../Bin/Data/OBJ/Lobby/OBJData", OBJECTTAG::BUILD_OBJ);

	m_mapLayer.insert({ eLayerTag, m_pLayer });

	return S_OK;
}

HRESULT CLobbyStage::Ready_Layer_GameLogic(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = m_pGLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	{
		// Player
		CGameObject* pPlayer = m_pPlayer = pGameObject = Management()->Get_Player();
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
		pPlayer->Get_Transform()->Set_Pos(20.f,5.0f,20.0f);

		//FootRay
		pGameObject = CFootRay::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);
		dynamic_cast<CFootRay*>(pGameObject)->Set_Host(pPlayer);

		//
		//pGameObject = CRay::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);
		//pGameObject = CStage1Boss::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BOSS, pGameObject), E_FAIL);

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
	}

	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}

HRESULT CLobbyStage::Ready_Layer_Camera(LAYERTAG eLayerTag)
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

HRESULT CLobbyStage::Ready_Layer_UI(LAYERTAG eLayerTag)
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

	//승용

	//Load_UI();

	//pGameObject = CImGuiManager::GetInstance()->Get_UI(L"Checkmark.png");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	//pGameObject = CUIMgr::GetInstance()->Get_UI(L"electricity-icon.png");
	//pGameObject = CImGuiManager::GetInstance()->Get_ImguiUI();




	return S_OK;
}

HRESULT CLobbyStage::Ready_Layer_MINIGAME(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//Engine::CGameObject* pGameObject = nullptr;


	m_mapLayer.insert({ eLayerTag, pLayer });

	return S_OK;
}


HRESULT CLobbyStage::Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag)
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
	OBJData* MovingTemp = nullptr;

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
				LightTemp = new OBJData;
				LightTemp = iter;
				m_VecLight.push_back(LightTemp);
			}
			if (iter->eOBJ_Attribute == OBJ_ATTRIBUTE::STD_OBJ) //Moving_OBJ에 담긴 값이 이상해서 STD_OBJ로 테스트
			{
				MovingTemp = new OBJData;
				MovingTemp = iter;
				m_VecMoving.push_back(MovingTemp);
			}

			m_iOBJIndex++;
		}
		m_mapLayer.emplace(LAYERTAG::ENVIRONMENT, m_pLayer);
	}

	if (pTag != nullptr) {
		delete[] pTag;
		pTag = nullptr;
	}

	if (LightTemp != nullptr) {
		delete LightTemp;
		LightTemp = nullptr;
	}

	if (MovingTemp != nullptr) {
		delete MovingTemp;
		MovingTemp = nullptr;
	}
	return S_OK;
}


HRESULT CLobbyStage::Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag)
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

HRESULT CLobbyStage::Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag)
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



void CLobbyStage::Admin_KeyInput()
{

	if (Engine::Get_DIKeyState(DIK_F9) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnPause(false, SCENETAG::LOBBY);
		m_bAdminSwitch = false;
	}

	if (Engine::Get_DIKeyState(DIK_M) & 0x80 && m_bAdminSwitch)
	{
		CLoadingStage1* pScene = nullptr;
		pScene = CLoadingStage1::Create(m_pGraphicDev, SCENETAG::BOSS_STAGE);

		//CUIMgr::GetInstance()->DestroyInstance();
		Management()->Get_Player()->Set_SceneChange(true);
		Management()->Set_SYSceneChange(true);
		Management()->Change_Scene(pScene);


		m_bAdminSwitch = false;
	}

	_bool CheckTemp = dynamic_cast<CPlayer*>(m_pPlayer)->Get_TriggerCheck();

	if ((Engine::Get_DIKeyState(DIK_F) & 0x80) && (CheckTemp == true)
		&& (CEventMgr::GetInstance()->Get_MiniGameState() == CEventMgr::MiniGameState::NOT_PLAY))
	{
		CEventMgr::GetInstance()->Set_MiniGameMode();

		if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == false)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == false)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == false))
		{
			SoundMgr()->StopSound(SOUND_BGM);
			CEventMgr::GetInstance()->OnMiniGame_Arrow(m_pGraphicDev, SCENETAG::LOBBY);
			SoundMgr()->PlayBGM(L"MiniGame1BGM.mp3",1.f);
		}
		else if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == false)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == false))
		{
			SoundMgr()->StopSound(SOUND_BGM);
			CEventMgr::GetInstance()->OnMiniGame_KickBoard(m_pGraphicDev, SCENETAG::LOBBY);
			SoundMgr()->PlayBGM(L"MiniGame2BGM.mp3", 1.f);
		}
		else if ((CEventMgr::GetInstance()->Get_MiniGameClearCheck(1) == true)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(2) == false)
			&& (CEventMgr::GetInstance()->Get_MiniGameClearCheck(0) == true))
		{
			SoundMgr()->StopSound(SOUND_BGM);
			CEventMgr::GetInstance()->OnMiniGame_Quiz(m_pGraphicDev, SCENETAG::LOBBY);
			SoundMgr()->PlayBGM(L"QuizBGM.mp3", 1.f);
		}
		else
		{
			SoundMgr()->StopSound(SOUND_BGM);
			SoundMgr()->PlayBGM(L"LobbyBGM3.mp3", 1);
		}

		m_bAdminSwitch = false;
	}
}

CLobbyStage* CLobbyStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLobbyStage* pInstance = new CLobbyStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossStage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLobbyStage::Free()
{
	SoundMgr()->StopAll();

	for (int i = 0; i < m_VecCubeData.size(); ++i)
	{
		Safe_Delete(m_VecCubeData[i]);
	}
	m_VecCubeData.clear();

// 	for (int i = 0; i < m_VecOBJData.size(); ++i)
// 	{
// 		Safe_Delete(m_VecOBJData[i]);
// 	}
// 	m_VecOBJData.clear();

	//for (int i = 0; i < m_VecCreatePoint.size(); ++i)
	//{
	//	Safe_Delete(m_VecCreatePoint[i]);
	//}
	//m_VecCreatePoint.clear();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	__super::Free();
}
