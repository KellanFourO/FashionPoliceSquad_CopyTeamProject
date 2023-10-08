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
#include "Rifle.h"
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

// TODO - 승용
#include "MyUI.h"
#include "CardList.h"
#include "UICamera.h"
#include "MyDialog.h"
#include "Portrait.h"
#include "CrossHair.h"
#include "MissionObjective.h"
#include "PaintShotGun.h"
#include "TailorAssertRifle.h"


class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

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

private:
	CLayer*				m_pLayer = nullptr;

	//////////////////////유진 함수, 변수////////////////////////
public:
	vector<CUBE*>&		Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*> pVecCubeData) { m_VecCubeData = pVecCubeData; }

	vector<OBJData*>&	Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*> pVecOBjData) { m_VecOBJData = pVecOBjData; }

	virtual HRESULT		Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag);

	vector<IDirect3DCubeTexture9*>& Get_VecTempCube() { return m_VecTempCube; }
	vector<IDirect3DBaseTexture9*>& Get_VecTempPlane() { return m_VecTempPlane; }

private:
	//BuildOBJ내에서 텍스쳐넘버 구분용으로 씀
	vector<IDirect3DCubeTexture9*>	m_VecTempCube;
	vector<IDirect3DBaseTexture9*>	m_VecTempPlane;

	vector<OBJData*>				m_VecOBJData;
	_uint							m_iOBJIndex = 0;

	vector<CUBE*>					m_VecCubeData;
	_uint							m_iCubeIndex = 0;

	SORTCUBE*						m_defSortCube = nullptr;//큐브 정렬용
	vector<SORTCUBE*>				m_pCubeForSort;			//큐브 정렬용
	SORTTEX*						m_defSortTex = nullptr; //텍스쳐 정렬용
	vector<SORTTEX*>				m_pTexForSort;			//텍스쳐 정렬용


	/////////////////////승용 함수, 변수/////////////////////////////////
public:
	HRESULT				Load_UI();
	void				Admin_KeyInput();


	////////////////////////준호 함수, 변수//////////////////////////////
	_bool				m_bSceneChange;
	_bool				m_bAdminSwitch = true; // 어드민키인풋 레벨업 통제용
	_float				m_fAdminTick = 0.f;	   // 어드민키인풋 레벨업 통제용


public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void		Free();
};

