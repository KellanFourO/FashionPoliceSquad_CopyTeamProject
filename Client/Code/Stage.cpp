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

	Load_Data_C_T(L"../Bin/Data/CPoint/CPointData", OBJECTTAG::BUILD_OBJ); //TODO

	srand(GetTickCount64());

	//TODO - �¿��߰� ũ�ν���� �߰�, �⺻ Ŀ�� �Ⱥ��̰�
	ShowCursor(FALSE);

	return S_OK;
}



_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);


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
	//mainApp ���� stage-Maptool �ܿ��� �ѱ�
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

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

	Load_Data(L"../Bin/Data/Map/MapData", OBJECTTAG::BUILD_CUBE);		// ť���ؽ�
	Load_Data(L"../Bin/Data/OBJ/OBJData", OBJECTTAG::BUILD_OBJ);		//TODO ��� �����ִ� ť���ؽ��� rcTEXT
		
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
		CGameObject* pPlayer = pGameObject = Management()->Get_Player();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER, pGameObject), E_FAIL);	//�÷��̾�

		pGameObject = Management()->Get_ShotGun();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);


		pGameObject = Management()->Get_Rifle();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_GUN, pGameObject), E_FAIL);


		pGameObject = Management()->Get_RifleHand();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_HAND, pGameObject), E_FAIL);

		//FootRay
		//pGameObject = CFootRay::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::RAY, pGameObject), E_FAIL);
		//dynamic_cast<CFootRay*>(pGameObject)->Set_Host(pPlayer);

		pGameObject = Management()->Get_ShotGunFlash();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);	//���Ǽ���

		pGameObject = Management()->Get_RifleFlash();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);	//������ ����

		pGameObject = Management()->Get_Lazer();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::PLAYER_LAZER, pGameObject), E_FAIL);



		pGameObject = Management()->Get_Belt();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::OBJECT, pGameObject), E_FAIL);	//��Ʈ
		// ��Ʈ��

		//pGameObject = CInventory::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::ITEM, pGameObject), E_FAIL);
		//dynamic_cast<CInventory*>(pGameObject)->Set_Player(dynamic_cast<CPlayer*>(pPlayer));	//�κ��丮



	}
	{
		//for (int i = 0; i < 6; i++) {
		//	pGameObject = CItem::Create(m_pGraphicDev);
		//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//	Item_Info Item_Info;
		//	Item_Info.Item_ID = (ItemID)i;
		//	Item_Info.vPos = _vec3(20 + 6 * i, 2.f, 35);
		//	dynamic_cast<CItem*>(pGameObject)->Set_INFO(Item_Info);
		//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::ITEM, pGameObject), E_FAIL);	//������ ��ġ
		//}
	}
	// ������

	//����
	//{
	//	pGameObject = CStage1Boss::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BOSS, pGameObject), E_FAIL);
	//}

	//pGameObject = CStage1Boss::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::BOSS, pGameObject), E_FAIL);



	for (int i = 0; i < 5; ++i)
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

//����Ʈ ��ƼŬ
	{
		//����Ʈ
		pGameObject = CMonsterBombEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);

		pGameObject = CMBulletBombEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::EFFECT, pGameObject), E_FAIL);

		//��ƼŬ
		pGameObject = CDustGrey::Create(m_pGraphicDev, _vec3(0.f, 0.f, 0.f), 256);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::PARTICLE, pGameObject), E_FAIL);

		pGameObject = CMBulletExplosion::Create(m_pGraphicDev, _vec3(0.f, 0.f, 0.f), 256);
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

	//pGameObject = CNewFPSCamera::Create(m_pGraphicDev,
	//	&_vec3(0.f, 10.f, -10.f),
	//	&_vec3(0.f, 0.f, 1.f),
	//	&_vec3(0.f, 1.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::FPSCAMERA, pGameObject), E_FAIL);

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





	m_mapLayer.insert({ eLayerTag, pLayer });

	//�¿�
	//Load_UI();

	//pGameObject = CImGuiManager::GetInstance()->Get_UI(L"Checkmark.png");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);

	//pGameObject = CUIMgr::GetInstance()->Get_UI(L"electricity-icon.png");
	//pGameObject = CImGuiManager::GetInstance()->Get_ImguiUI();




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

HRESULT CStage::Load_Data_C_T(const TCHAR* pFilePath, OBJECTTAG eTag)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile) { return E_FAIL; }

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	string expectedTag;

	CGameObject* pGameObject = nullptr;
	CLayer* targetLayer = nullptr;

	C_POINT* pOBJ = nullptr;
	TRIGGER* pTR = nullptr;

	if (eTag == OBJECTTAG::BUILD_OBJ) {
		expectedTag = "CPointData";
		targetLayer = m_pLayer;
	}
	else if (eTag == OBJECTTAG::TRIGGER) {
		expectedTag = "TriggerData";
		targetLayer = m_pGLayer;
	}

	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	CHAR* pTag = new CHAR[dwStrByte];

	ReadFile(hFile, pTag, dwStrByte, &dwByte, nullptr);
	string m_strText(pTag);

	while (true)
	{
		if (eTag == OBJECTTAG::BUILD_OBJ) {
			pOBJ = new C_POINT;
			ReadFile(hFile, pOBJ, sizeof(C_POINT), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pOBJ);
				break;
			}
			m_VecCreatePoint.push_back(pOBJ);
		}
		else if (eTag == OBJECTTAG::TRIGGER) {
			TRIGGER* pTR = new TRIGGER;
			ReadFile(hFile, pTR, sizeof(TRIGGER), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pTR);
				break;
			}
			m_TriggerDataTemp.push_back(pTR);
		}
	}

	CloseHandle(hFile);

	for (auto& data : m_VecCreatePoint) {
		C_POINT* pOBJ = static_cast<C_POINT*>(data);
		pGameObject = CBuild_Obj::Create(m_pGraphicDev, pOBJ->defOBJData.vPos, pOBJ->defOBJData.uiTextureNum,
		pOBJ->defOBJData.vSize, pOBJ->defOBJData.iRotateCount, m_iOBJIndex, pOBJ->defOBJData.eOBJ_TYPE, pOBJ->defOBJData.eOBJ_Attribute);
				
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(targetLayer->Add_GameObject(eTag, pGameObject), E_FAIL);

		if (eTag == OBJECTTAG::BUILD_OBJ) {
			m_iOBJIndex++;
		}
	}
	for (auto data : m_TriggerDataTemp)
	{
		TRIGGER* pTR = static_cast<TRIGGER*>(data);
		pGameObject = CTrigger::Create(m_pGraphicDev, pTR->vPos, pTR->iIndex, pTR->vSize, pTR->eTrCase, pTR->eTrType, pTR->eTrName);

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(targetLayer->Add_GameObject(eTag, pGameObject), E_FAIL);

	}

	delete[] pTag;
	pTag = nullptr;

	if (eTag == OBJECTTAG::BUILD_OBJ) {
		m_mapLayer.emplace(LAYERTAG::ENVIRONMENT, m_pLayer);
	}
	else if (eTag == OBJECTTAG::TRIGGER) {
		m_mapLayer.emplace(LAYERTAG::GAMELOGIC, m_pGLayer);
	}
	return S_OK;
}



// HRESULT CStage::Load_UI()
// {
// 	Engine::CGameObject* pGameObject = nullptr;
//
// 	wifstream fin;
//
// 	fin.open(L"../Bin/Data/UI/UIdata.dat");
//
// 	if (!fin.fail())
// 	{
// 		wstring line;
//
// 		while (getline(fin, line)) // �� �پ� �б�
// 		{
// 			wistringstream iss(line);
// 			wstring token;
//
// 			_tchar szKey[MAX_PATH] = L"";
// 			_tchar szSizeX[MAX_PATH] = L"";
// 			_tchar szSizeY[MAX_PATH] = L"";
// 			_tchar szSizeZ[MAX_PATH] = L"";
// 			_tchar szPosX[MAX_PATH] = L"";
// 			_tchar szPosY[MAX_PATH] = L"";
// 			_tchar szPosZ[MAX_PATH] = L"";
//
// 			getline(iss, token, L','); // ','�� �����ڷ� ����Ͽ� ���� �ڸ�
// 			_tcscpy_s(szKey, MAX_PATH, token.c_str());
//
// 			getline(iss, token, L',');
// 			_tcscpy_s(szSizeX, MAX_PATH, token.c_str());
//
// 			getline(iss, token, L',');
// 			_tcscpy_s(szSizeY, MAX_PATH, token.c_str());
//
// 			getline(iss, token, L',');
// 			_tcscpy_s(szSizeZ, MAX_PATH, token.c_str());
//
// 			getline(iss, token, L',');
// 			_tcscpy_s(szPosX, MAX_PATH, token.c_str());
//
// 			getline(iss, token, L',');
// 			_tcscpy_s(szPosY, MAX_PATH, token.c_str());
//
// 			getline(iss, token, L',');
// 			_tcscpy_s(szPosZ, MAX_PATH, token.c_str());
//
// 			if (wcsstr(szKey, L"des"))
// 			{
// 				continue;
// 			}
// 			pGameObject = CUIMgr::GetInstance()->Get_UI(szKey);
// 			NULL_CHECK_RETURN(pGameObject, E_FAIL);
// 			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(OBJECTTAG::UI, pGameObject), E_FAIL);
//
// 		}
// 	}
// 	fin.close();
//
// 		m_mapLayer.insert({ LAYERTAG::UI, m_pLayer });
//
// 	return S_OK;
// }

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

	for (int i = 0; i < m_VecCreatePoint.size(); ++i)
	{
		Safe_Delete(m_VecCreatePoint[i]);
	}
	m_VecCreatePoint.clear();


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	__super::Free();
}
