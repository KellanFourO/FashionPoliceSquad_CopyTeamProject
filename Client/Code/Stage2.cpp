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
#include <windows.h>



CStage2::CStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

CStage2::~CStage2()
{
}

HRESULT CStage2::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(LAYERTAG::CAMERA), E_FAIL);

	Add_Light();

	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	srand(GetTickCount64());

	Load_Data_T(L"../Bin/Data/Trigger/Stage2/TriggerData", OBJECTTAG::O_TRIGGER);

	//TODO - 승용추가 크로스헤어 추가, 기본 커서 안보이게
	ShowCursor(FALSE);

	CSoundMgr::GetInstance()->PlayBGM(L"NightBGM1.wav", 0.25);
	m_eSceneTag = SCENETAG::STAGE2;
	return S_OK;
}

_int CStage2::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);
 	if (m_bLateInit)
 	{
 		CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::STAGE2, DIALOGTAG::STORY_ST2_INTRO);
 		CEventMgr::GetInstance()->OnPause(true, SCENETAG::STAGE2);
 		m_bLateInit = false;
 	}


	//if (m_pLayer->Get_ObjectList(OBJECTTAG::MONSTER).size() == 0 && m_bEnd)
	//{
	//	SoundMgr()->StopAll();
	//	CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::STAGE2, DIALOGTAG::STORY_ST2_CONCLU);
	//	CEventMgr::GetInstance()->OnPause(true, SCENETAG::STAGE2);
	//	SoundMgr()->PlayBGM(L"EndBGM.wav",1.f);
	//	m_bEnd = false;
	//}
	//
	//if (!m_bEnd)
	//{
	//	if (!m_bVideo)
	//	{
	//		m_hVideo = MCIWndCreate(g_hWnd,			// 부모 윈도우의 핸들
	//			nullptr,		// MCI윈도우를 사용하는 인스턴스 핸들
	//			WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, // 윈도우 모양 설정(WS_CHILD : 자식 창 형태, WS_VISIBLE : 그 즉시 그리기, MCIWNDF_NOPLAYBAR : 플레이바를 출력하지 않음)
	//			L"../Bin/Resource/Video/Wildlife.wmv"); // 재생할 파일의 경로
	//
	//// MoveWindow : 동영상을 재생할 크기를 설정, 설정하지 않을 경우 동영상의 원래 크기대로 출력
	//		MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);
	//
	//		MCIWndPlay(m_hVideo);
	//
	//		m_bVideo = true;
	//	}
	//
	//	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo)
	//		|| Key_Down(DIK_RETURN))
	//	{
	//		return 0;
	//	}
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

void CStage2::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	CollisionManager()->LateUpdate_Collision();

	Check_Trigger();
	Check_Collision_Water();

}

void CStage2::Render_Scene()
{
}

HRESULT CStage2::Ready_LightInfo()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//mainApp 말고 stage-Maptool 단에서 켜기
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f, 0.6f, 0.4f, 1.f };     // 색깔(난반사)
	tLightInfo.Specular = { 1.f, 0.6f, 0.4f, 1.f };    // 정반사
	tLightInfo.Ambient = { 1.f, 0.6f, 0.4f, 1.0f };    //조명에 의한 환경반사
	tLightInfo.Direction = { 1.f, -1.f, 1.f }; //방향

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage2::Ready_Prototype()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UITex", CUITex::Create(m_pGraphicDev)), E_FAIL);
	return S_OK;
}

HRESULT CStage2::Ready_Layer_Environment(LAYERTAG eLayerTag)
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

	Load_Data(L"../Bin/Data/Map/Stage2/MapData", OBJECTTAG::BUILD_CUBE);
	Load_Data(L"../Bin/Data/OBJ/Stage2/OBJData", OBJECTTAG::BUILD_OBJ);

	m_mapLayer.insert({ eLayerTag, m_pLayer });

	return S_OK;
}

HRESULT CStage2::Ready_Layer_GameLogic(LAYERTAG eLayerTag)
{
	Engine::CLayer* pLayer = m_pGLayer = Engine::CLayer::Create(eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	Load_Data_C(L"../Bin/Data/CPoint/Stage2/C0/CPointData", OBJECTTAG::BUILD_OBJ, 0);

	{
		// Player
		m_pPlayer = pGameObject = Management()->Get_Player();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER, pGameObject), E_FAIL);	//플레이어

		dynamic_cast<CPlayer*>(m_pPlayer)->ClearGunList();


		pGameObject = Management()->Get_ShotGun();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);


		pGameObject = Management()->Get_Rifle();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);

		//pGameObject = Management()->Get_RifleHand();
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_HAND, pGameObject), E_FAIL);

		dynamic_cast<CPlayer*>(m_pPlayer)->Set_SceneChange(false);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetGun(pLayer);

		_vec3 vPlayerPos, vPlayerLook;
		m_pPlayer->Get_Transform()->Set_Pos(198.5f,12.0f,249.5f);
		vPlayerPos = { 198.5f, 12.0f, 249.5f };

		if (m_Starting_Point != nullptr) {
			vPlayerLook = m_Starting_Point->vPos - vPlayerPos;
			D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
			m_pPlayer->Get_Transform()->Set_Look(vPlayerLook);
			int i = 0;
		}

//  		vPlayerLook = {0.f, 1.f, 0.f};
//  		m_pPlayer->Get_Transform()->Set_Look(vPlayerLook);
// 		int i = 0;

		//FootRay
		pGameObject = CFootRay::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);
		dynamic_cast<CFootRay*>(pGameObject)->Set_Host(m_pPlayer);

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

HRESULT CStage2::Ready_Layer_Camera(LAYERTAG eLayerTag)
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

HRESULT CStage2::Ready_Layer_UI(LAYERTAG eLayerTag)
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

	pGameObject = CRecognitionRange::Create(m_pGraphicDev, nullptr, UI_TYPE::DESTINATION);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CRecognitionRange*>(pGameObject)->Set_TargetPos(_vec3(64.965f, 2.f, 50.675f));
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

HRESULT CStage2::Add_Light()
{
	if (!m_VecLight.empty()) {
		const int LightNum = m_VecLight.size();
		D3DLIGHT9* tLightInfoo = new D3DLIGHT9[LightNum];

		for (int i = 0; i != LightNum; ++i)
		{
			ZeroMemory(&tLightInfoo[i], sizeof(D3DLIGHT9));

			tLightInfoo[i].Type = D3DLIGHT_POINT;

			tLightInfoo[i].Diffuse = { 0.8f, 0.5f, 0.3f, 0.4f };     // 색깔(난반사)
			tLightInfoo[i].Specular = { 0.8f, 0.5f, 0.3f, 0.4f };    // 정반사
			tLightInfoo[i].Ambient = { 0.8f, 0.5f, 0.3f, 0.4f };    // 환경반사

			_vec3 Height = { 0.f, 5.f, 0.f };
			_float Distance = 30.f;

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

HRESULT CStage2::Light_OnOff_Check()
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


HRESULT CStage2::Check_Trigger()
{
	if (!m_TriggerDataTemp.empty())
	{
		int iCountNum = -1;
		_vec3 PlayerPos;
		Management()->Get_Player()->Get_Transform()->Get_Info(INFO_POS, &PlayerPos);

		for (auto& iter : m_TriggerDataTemp)
		{
			_float MinX = (iter->vPos.x) - (iter->vSize.x * 0.5f);
			_float MaxX = (iter->vPos.x) + (iter->vSize.x * 0.5f);

			_float MinZ = (iter->vPos.z) - (iter->vSize.z * 0.5f);
			_float MaxZ = (iter->vPos.z) + (iter->vSize.z * 0.5f);

			if (((PlayerPos.x >= MinX) && (PlayerPos.x <= MaxX))
				&& ((PlayerPos.z >= MinZ) && (PlayerPos.z <= MaxZ)))
			{
				if ((iter->eTrName == TRIGGER_NUMBER::TR0) && (iter->eTrSTATE == TRIGGER_STATE::TR_BEFORE))
				{
					iCountNum = 1;
					Load_Data_C(L"../Bin/Data/CPoint/Stage2/C1/CPointData", OBJECTTAG::BUILD_OBJ, iCountNum);
					iter->eTrSTATE = TRIGGER_STATE::TR_AFTER;
					Create_Monster(iCountNum);
				}
				else if ((iter->eTrName == TRIGGER_NUMBER::TR1) && (iter->eTrSTATE == TRIGGER_STATE::TR_BEFORE))
				{
					iCountNum = 2;
					Load_Data_C(L"../Bin/Data/CPoint/Stage2/C2/CPointData", OBJECTTAG::BUILD_OBJ, iCountNum);
					iter->eTrSTATE = TRIGGER_STATE::TR_AFTER;
					Create_Monster(iCountNum);
				}
				else if ((iter->eTrName == TRIGGER_NUMBER::TR3) && (iter->eTrSTATE == TRIGGER_STATE::TR_BEFORE))
				{
					iCountNum = 3;
					Load_Data_C(L"../Bin/Data/CPoint/Stage2/C3/CPointData", OBJECTTAG::BUILD_OBJ, iCountNum);
					iter->eTrSTATE = TRIGGER_STATE::TR_AFTER;
					Create_Monster(iCountNum);
				}
			}
		}
	}
	return S_OK;
}


HRESULT CStage2::Check_Collision_Water()
{

	return S_OK;
}


HRESULT CStage2::Create_Monster(int iNum)
{
	Engine::CGameObject* pGameObject = nullptr;

	for (auto& iter : m_VecCreatePoint[iNum])
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

	m_mapLayer.emplace(LAYERTAG::GAMELOGIC, m_pLayer);

	return S_OK;
}




HRESULT CStage2::Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag)
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
	OBJData* FogTemp = nullptr;

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

			if (iter->eOBJ_Attribute == OBJ_ATTRIBUTE::FOG_OBJ)
			{
				OBJData* FogTemp = new OBJData;
				FogTemp = iter;
				m_VecWater.push_back(FogTemp);
			}

			if (iter->eOBJ_Attribute == OBJ_ATTRIBUTE::STD_OBJ)
			{
				OBJData* STDTemp = new OBJData;
				STDTemp = iter;
				m_Starting_Point = STDTemp;
			}

			m_iOBJIndex++;
		}
		m_mapLayer.emplace(LAYERTAG::ENVIRONMENT, m_pLayer);
	}

	delete[] pTag;
	pTag = nullptr;

	delete LightTemp;
	LightTemp = nullptr;

	delete FogTemp;
	FogTemp = nullptr;

	return S_OK;
}


HRESULT CStage2::Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag, int CountNum)
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

HRESULT CStage2::Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag)
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


void CStage2::Admin_KeyInput()
{

	if (Engine::Get_DIKeyState(DIK_F4) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnPause(true, SCENETAG::STAGE);
		m_bAdminSwitch = false;
	}
	if (Engine::Get_DIKeyState(DIK_F5) & 0x80 && m_bAdminSwitch)
	{
		CEventMgr::GetInstance()->OnPause(false, SCENETAG::STAGE);
		m_bAdminSwitch = false;
	}

	if (Key_Up(DIK_U))
	{
		CEventMgr::GetInstance()->OffDialog();
	}
}



CStage2* CStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage2* pInstance = new CStage2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Stage2 Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStage2::Free()
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

	MCIWndClose(m_hVideo);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	__super::Free();
}
