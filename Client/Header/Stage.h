#pragma once

#include "Scene.h"

// TODO - ����
#include "FlyingCamera.h"
#include "FPSCamera.h"
#include "NewFPSCamera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Build_Cube.h"
#include "Build_Obj.h"

// TODO - �翬
#include "Player.h"
#include "Monster.h"
#include "BigDaddyMonster.h"
#include "DullSuitMonster.h"
#include "KickBoard.h"
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


// TODO - ����? ��ȣ��? �¿�?
#include "Effect.h"
#include "Engine_Define.h"

// TODO - �¿�
#include "MyUI.h"
#include "CardList.h"
#include "UICamera.h"
#include "MyDialog.h"
#include "Portrait.h"
#include "CrossHair.h"
#include "MissionObjective.h"


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


	// TODO - ���� �Լ�
public:
	vector<CUBE*>&		Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*> pVecCubeData) { m_VecCubeData = pVecCubeData; }

	vector<OBJData*>&	Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*> pVecOBjData) { m_VecOBJData = pVecOBjData; }

	virtual HRESULT		Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag);



	// TODO - �¿� �Լ�
public:
	HRESULT				Load_UI();
	void				Admin_KeyInput();


private:
	CLayer*				m_pLayer = nullptr;

	// TODO - ���� ����
	vector<OBJData*>	m_VecOBJData;
	_uint				m_iOBJIndex = 0;
	vector<CUBE*>		m_VecCubeData;
	_uint				m_iCubeIndex = 0;

	//��ȣ
	_bool				m_bSceneChange;
	_bool				m_bAdminSwitch = true; // ����Ű��ǲ ������ ������
	_float				m_fAdminTick = 0.f;	   // ����Ű��ǲ ������ ������


public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void		Free();
};

