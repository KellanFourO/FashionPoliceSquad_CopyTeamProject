#pragma once
#include "Scene.h"

// TODO - 유진
#include "FlyingCamera.h"
#include "FPSCamera.h"
#include "NewFPSCamera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Build_Cube.h"
#include "Build_Obj.h"

// TODO - 재연
#include "Player.h"
#include "Monster.h"
#include "BigDaddyMonster.h"
#include "DullSuitMonster.h"
#include "KickBoardMonster.h"
#include "Dyehard.h"
#include "Belt.h"
#include "MuzzleFlash.h"
#include "MuzzlefFlash_Rifle.h"
#include "PaintBall.h"
#include "Item.h"
#include "Inventory.h"
#include "Stage1Boss.h"

#include "Lazer.h"


// TODO - 공동? 준호형? 승용?
#include "Effect.h"
#include "Engine_Define.h"


//TODO 승용
#include "UI_MyDialog.h"
#include "UI_Portrait.h"

#include "UI_CardList.h"
#include "UI_CrossHair.h"

#include "PaintShotGun.h"
#include "TailorAssertRifle.h"
#include "TailorAssertRifleHand.h"

#include "UI_MissionObjective.h"


class CLobbyStage : public Engine::CScene
{
private:
	explicit CLobbyStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLobbyStage();

public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		LateUpdate_Scene();
	virtual void		Render_Scene();

private:
	HRESULT				Ready_LightInfo();
	HRESULT				Ready_Prototype();
	HRESULT				Ready_Layer_Environment(LAYERTAG eLayerTag);
	HRESULT				Ready_Layer_GameLogic(LAYERTAG eLayerTag);
	HRESULT				Ready_Layer_Camera(LAYERTAG eLayerTag);
	HRESULT				Ready_Layer_UI(LAYERTAG eLayerTag);

	HRESULT				Add_Light();
	HRESULT				Light_OnOff_Check();

	HRESULT				Moving_Wall();

private:
	CLayer* m_pLayer = nullptr;  //Environment
	CLayer* m_pGLayer = nullptr; //GameLogic
	CGameObject* m_pPlayer = nullptr;

	_bool	m_bLateInit = true;

	//////////////////////유진 함수, 변수////////////////////////
public:
	vector<CUBE*>&		Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*> pVecCubeData) { m_VecCubeData = pVecCubeData; }

	vector<OBJData*>&	Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*> pVecOBjData) { m_VecOBJData = pVecOBjData; }

	virtual HRESULT		Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag);
	HRESULT				Load_Data_C(const TCHAR* pFilePath, OBJECTTAG eTag);
	HRESULT				Load_Data_T(const TCHAR* pFilePath, OBJECTTAG eTag);

	vector<IDirect3DCubeTexture9*>& Get_VecTempCube() { return m_VecTempCube; }
	vector<IDirect3DBaseTexture9*>& Get_VecTempPlane() { return m_VecTempPlane; }

private:
	//BuildOBJ내에서 텍스쳐넘버 구분용으로 씀
	vector<IDirect3DCubeTexture9*>	m_VecTempCube;
	vector<IDirect3DBaseTexture9*>	m_VecTempPlane;

	vector<OBJData*>				m_VecOBJData;
	_uint							m_iOBJIndex = 0;

	vector<C_POINT*>			    m_VecCreatePoint;
	vector<TRIGGER*>			    m_TriggerDataTemp;

	vector<OBJData*>				m_VecLight;
	vector<OBJData*>				m_VecMoving;

	_float							MovingIndextemp;
	_bool							MovingCheck = false;
	_float							MovingIndextemp2;
	_bool							MovingCheck2 = false;

	vector<CUBE*>					m_VecCubeData;
	_uint							m_iCubeIndex = 0;

	SORTCUBE* m_defSortCube = nullptr;//큐브 정렬용
	vector<SORTCUBE*>				m_pCubeForSort;			//큐브 정렬용
	SORTTEX* m_defSortTex = nullptr; //텍스쳐 정렬용
	vector<SORTTEX*>				m_pTexForSort;			//텍스쳐 정렬용

public:
	void				Admin_KeyInput();

	_bool				m_bAdminSwitch = true; // 어드민키인풋 레벨업 통제용
	_float				m_fAdminTick = 0.f;	   // 어드민키인풋 레벨업 통제용

public:
	static CLobbyStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();
};

