#include "stdafx.h"
#include "..\Header\Loading.h"

#include "Export_Utility.h"
#include "ImGuiManager.h"
#include "UIMgr.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
	//Free();
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt); // 크리티컬섹션 Init

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eLoadingID = eID;

	return S_OK;
}

_uint CLoading::Loading_For_Stage()
{

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX_DOUBLE, VTXCNTZ_DOUBLE, VTXITV)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RigidBody", CRigidBody::Create(m_pGraphicDev)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlaneTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTex", CEffectTex::Create(m_pGraphicDev)), E_FAIL)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/Obj/CubeType/Box_Full%d.dds", OBJ_TYPE::CUBE_TYPE, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlaneTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Obj/PlaneType/PlaneType_%d.png", OBJ_TYPE::PLANE_TYPE, 57)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SYTex", CSYTex::Create(m_pGraphicDev)), E_FAIL);
	Set_Value(3);



	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/MainTile_%d.png", 1)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/Sky%d.dds", 2)), E_FAIL);
	Set_Value(3);
	//Effect
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterBombEffectTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/MonsterBomb/MonsterBomb%d.png", 15)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player0.png")), E_FAIL);
	Set_Value(3);


	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_suit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/loose-suit-spritesheet.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BrifCase", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/briefcase-projectile-02.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BrifCase2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/briefcase-projectile.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_suit_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/dull_suit4.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KickBoardMonster", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/neonshirt-v1_Resize.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Stage1Boss", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/boss 1 - hugo bauss sprite1.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Stage1Bullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/gold-bar-projectile_0.png")), E_FAIL);
		Set_Value(3);
	}
	// 몬스터 관련 Ready_Proto

	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dyehard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-dyehard2.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-tailormade-body2.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RifleHand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/tailormade-hand1.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Belt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Belt.png")), E_FAIL);
		Set_Value(3);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_Spiner", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-tailormade-spinner.png")), E_FAIL);
		Set_Value(3);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MuzzleFlash", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/muzzleflash2.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MuzzleFlash2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/muzzleflash1.png")), E_FAIL);
		Set_Value(3);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PaintBall", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/paintballs.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PaintBulletTrace", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/paintballboom%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Lazer", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/laserbeam_R.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RifleBullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Rifle_Bullet.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Item", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/item-pickups.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sisors", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/scissors-pickup.png")), E_FAIL);
		Set_Value(3);
	}
	// 플레이어 관련 Ready_Proto

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Build_Cube_Texture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/Cube/CubeTest%d.dds", 21)), E_FAIL);
	Set_Value(3);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BaseUI", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/all wheels.png")), E_FAIL);
	Set_Value(3);

	//승용
	{
		CUIMgr::GetInstance()->Ready_UIMgr();
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DialogTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/DIALOUGE/DIALOG_1_Fix1.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PortraitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/PORTRAIT/portrait-des.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CrossHairTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/CROSSHAIR/UI-crosshair_2.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CardFrontTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/card_frame01.png")), E_FAIL);
		Set_Value(3);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CardBackTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/Card_Back.png")), E_FAIL);
		Set_Value(4);

	}

	m_bFinish = true;
	return 0;
}

_uint CLoading::Loading_For_MapTool()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX_DOUBLE, VTXCNTZ_DOUBLE, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlaneTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/Obj/CubeType/Box_Full%d.dds", OBJ_TYPE::CUBE_TYPE, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlaneTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Obj/PlaneType/PlaneType_%d.png", OBJ_TYPE::PLANE_TYPE, 57)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CursorTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/Cursor/MapCursor%d.dds", 1)), E_FAIL);


	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/MainTile_%d.png", 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WireTex", CWireTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WireTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/MainTile_%d.png", 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Build_Cube_Texture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/Cube/CubeTest%d.dds", 21)), E_FAIL);

	m_bFinish = true;

	return 0;
}

size_t CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_For_Stage();
		break;

	case LOADING_MAPTOOL:
		iFlag = pLoading->Loading_For_MapTool();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
