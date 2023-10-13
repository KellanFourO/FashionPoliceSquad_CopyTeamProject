#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"
#include "Layer.h"
#include "../Client/Header/Player.h"
#include "../Client/Header/NewFPSCamera.h"
#include "../Client/Header/PaintShotGun.h"
#include "../Client/Header/TailorAssertRifle.h"
#include "../Client/Header/TailorAssertRifleHand.h"
#include "../Client/Header/Belt.h"
#include "../Client/Header/MuzzleFlash.h"
#include "../Client/Header/MuzzlefFlash_Rifle.h"
#include "../Client/Header/Lazer.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent*				Get_Component(COMPONENTID eID, LAYERTAG eLayerTag, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag);
	CLayer*					Get_Layer(LAYERTAG eLayerTag)		{ return m_pScene->Get_Layer(eLayerTag); };
	vector<CGameObject*>&	Get_ObjectList(LAYERTAG eLayerTag, OBJECTTAG eObjTag)
	{
		return m_pScene->Get_Layer(eLayerTag)->Get_ObjectList(eObjTag);
	}


	map<SCENETAG, CScene*>	Get_MapScene()									{	return m_mapScene;	}
	void					Set_MapScene(map<SCENETAG, CScene*> pMapScene)	{ m_mapScene = pMapScene; }

	CScene* Get_One_Scene(SCENETAG eSceneTag) { return m_mapScene[eSceneTag]; }

public:
	HRESULT			Set_Scene(CScene* pScene);
	CScene*			Get_Scene() { return m_pScene; }

	_bool			Get_SYSceneChange() { return m_bSYSceneChange; }
	void			Set_SYSceneChange(_bool _bSceneChange) { m_bSYSceneChange = _bSceneChange; }

	CPlayer*			Get_Player() { return m_pPlayer; }
	void				Set_Player(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }

	CNewFPSCamera*		Get_Camera() { return m_pCamera; }
	void				Set_Camera(CNewFPSCamera* _pCamera) { m_pCamera = _pCamera;}

	CPaintShotGun*		Get_ShotGun() { return m_pShotGun;}
	void				Set_ShotGun(CPaintShotGun* _pShotgun) { m_pShotGun =_pShotgun;}

	CTailorAssertRifle* Get_Rifle() { return m_pRifle;}
	void				Set_Rifle(CTailorAssertRifle* _pRifle) { m_pRifle = _pRifle;}

	CTailorAssertRifleHand* Get_RifleHand() { return m_pRifleHand;}
	void					Set_RifleHand(CTailorAssertRifleHand* _pHand) { m_pRifleHand = _pHand;}

	CBelt*				Get_Belt() { return m_pBelt;}
	void				Set_Belt(CBelt* _pBelt) { m_pBelt = _pBelt;}

	CLazer*				Get_Lazer() { return m_pLazer;}
	void				Set_Lazer(CLazer* _pLazer) { m_pLazer = _pLazer;}

	CMuzzleFlash*		Get_ShotGunFlash() { return m_pShotGunFlash;}
	void				Set_ShotGunFlash(CMuzzleFlash* _pFlash) { m_pShotGunFlash = _pFlash;}

	CMuzzleFlash_Rifle* Get_RifleFlash() { return m_pRifleFlash;}
	void				Set_RifleFlash(CMuzzleFlash_Rifle* _pFlash) { m_pRifleFlash = _pFlash;}

	HRESULT			Change_Scene(CScene* pScene);
	HRESULT			Load_Data();
	_bool			Get_VisitScene(SCENETAG _SceneTag) {return m_bStageVisit[(_uint)_SceneTag];}

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	CScene*			m_pPreScene = nullptr;
	CScene*			m_pScene = nullptr;
	CScene*			m_pNextScene = nullptr;

	_bool           m_bGameStop=false;
	_bool			m_bSYSceneChange = false;
	_bool           m_bSceneChange =false;

	bool					m_bStageVisit[(_uint)SCENETAG::SCENETAG_END];
	CPlayer*				m_pPlayer = nullptr;
	CNewFPSCamera*			m_pCamera = nullptr;
	CPaintShotGun*			m_pShotGun = nullptr;
	CTailorAssertRifle*		m_pRifle = nullptr;
	CTailorAssertRifleHand* m_pRifleHand = nullptr;
	CBelt*					m_pBelt	=nullptr;
	CLazer*					m_pLazer = nullptr;

	CMuzzleFlash*			m_pShotGunFlash = nullptr;
	CMuzzleFlash_Rifle*		m_pRifleFlash = nullptr;

public:
	map<SCENETAG, CScene*>		m_mapScene;

	virtual void	Free();

};

END
