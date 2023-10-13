#pragma once

#include "Base.h"
#include "Layer.h"
#include "Frustum.h"
class CPlayer;


BEGIN(Engine)

class ENGINE_DLL CScene :public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene(LAYERTAG _eLayerTag);
	virtual ~CScene();

public:
	//Get
	SCENETAG					Get_SceneTag() { return m_eSceneTag; }
	CComponent*					Get_Component(COMPONENTID eID, LAYERTAG eLayerTag, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag);
	CLayer*						Get_Layer(LAYERTAG eLayerTag) { return m_mapLayer[eLayerTag]; };
	_bool						Get_ReadyCube() { return m_bReadyCube; }
	_bool						Get_Pause() { return m_bPause;}
	//Set
	void						Set_Pause(_bool _bPause) { m_bPause = _bPause; };

	vector<CGameObject*>&		Get_ObjectList(LAYERTAG eLayerTag, OBJECTTAG eObjTag)
	{
		return m_mapLayer[eLayerTag]->Get_ObjectList(eObjTag);
	}

	//map데이터 상속 으로 줄수있게
	virtual	HRESULT				Load_Data() { return S_OK; }
	CPlayer*					Get_MainPlayer() { return m_pPlayer; }

	void						Set_MainPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
	void						Set_SceneTag(SCENETAG eTag) { m_eSceneTag = eTag; }

public:
	virtual HRESULT				Ready_Scene();
	virtual _int				Update_Scene(const _float& fTimeDelta);
	virtual void				LateUpdate_Scene();
	virtual void				Render_Scene()PURE;


protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<LAYERTAG, CLayer*>		m_mapLayer;


	_bool						m_bReadyCube = false;
	_bool						m_bPause = false;
	SCENETAG					m_eSceneTag;
	CPlayer*					m_pPlayer;

public:
	virtual void	Free();
};

END