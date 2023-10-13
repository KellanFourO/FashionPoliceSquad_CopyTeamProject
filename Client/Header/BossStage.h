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


// TODO - ����? ��ȣ��? �¿�?
#include "Effect.h"
#include "Engine_Define.h"


//TODO �¿�
#include "UI_CardList.h"
#include "UICamera.h"
#include "UI_MyDialog.h"
#include "UI_Portrait.h"
#include "UI_CrossHair.h"
#include "UI_MissionObjective.h"

#include "UI_Berserk.h"
#include "UI_BerserkFrame.h"

#include "UI_HPBarValue.h"
#include "UI_HPBarFrame.h"
#include "UI_HPMark.h"

#include "UI_ShieldFrame.h"
#include "UI_ShieldValue.h"
#include "UI_ShieldMark.h"

#include "UI_PlayerFace.h"
#include "UI_Hat.h"

#include "UI_BulletInfoCount.h"
#include "UI_BulletInfoName.h"

#include "UI_WeaponInfo.h"

#include "PaintShotGun.h"
#include "TailorAssertRifle.h"
#include "TailorAssertRifleHand.h"


class CBossStage : public Engine::CScene
{
private:
	explicit CBossStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossStage();

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
	CLayer* m_pLayer = nullptr;
	_bool	m_bLateInit = true;

	//////////////////////���� �Լ�, ����////////////////////////
public:
	vector<CUBE*>& Get_VecCubeData() { return m_VecCubeData; }
	void				Set_VecCubeData(vector<CUBE*> pVecCubeData) { m_VecCubeData = pVecCubeData; }

	vector<OBJData*>& Get_VecOBJData() { return m_VecOBJData; }
	void				Set_VecOBJData(vector<OBJData*> pVecOBjData) { m_VecOBJData = pVecOBjData; }

	virtual HRESULT		Load_Data(const TCHAR* pFilePath, OBJECTTAG eTag);

	vector<IDirect3DCubeTexture9*>& Get_VecTempCube() { return m_VecTempCube; }
	vector<IDirect3DBaseTexture9*>& Get_VecTempPlane() { return m_VecTempPlane; }

private:
	//BuildOBJ������ �ؽ��ĳѹ� ���п����� ��
	vector<IDirect3DCubeTexture9*>	m_VecTempCube;
	vector<IDirect3DBaseTexture9*>	m_VecTempPlane;

	vector<OBJData*>				m_VecOBJData;
	_uint							m_iOBJIndex = 0;

	vector<CUBE*>					m_VecCubeData;
	_uint							m_iCubeIndex = 0;

	SORTCUBE* m_defSortCube = nullptr;//ť�� ���Ŀ�
	vector<SORTCUBE*>				m_pCubeForSort;			//ť�� ���Ŀ�
	SORTTEX* m_defSortTex = nullptr; //�ؽ��� ���Ŀ�
	vector<SORTTEX*>				m_pTexForSort;			//�ؽ��� ���Ŀ�

public:
	void				Admin_KeyInput();

	_bool				m_bAdminSwitch = true; // ����Ű��ǲ ������ ������
	_float				m_fAdminTick = 0.f;	   // ����Ű��ǲ ������ ������

public:
	static CBossStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();
};

