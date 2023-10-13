#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"
#include "Layer.h"
#include "../Client/Header/Player.h"

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

	CPlayer*		Get_Player() { return m_pPlayer; }
	void			Set_Player(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }

	HRESULT			Change_Scene(CScene* pScene);
	HRESULT			Load_Data();
	_bool			Get_VisitScene(SCENETAG _SceneTag) {return m_bStageVisit[(_uint)_SceneTag];}

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	CScene* m_pPreScene = nullptr;
	CScene* m_pScene = nullptr;
	CScene* m_pNextScene = nullptr;

	_bool           m_bGameStop=false;
	_bool			m_bSYSceneChange = false;
	_bool           m_bSceneChange =false;

	bool           m_bStageVisit[(_uint)SCENETAG::SCENETAG_END];
	CPlayer* m_pPlayer = nullptr;

public:
	map<SCENETAG, CScene*>		m_mapScene;

	virtual void	Free();

};

END
