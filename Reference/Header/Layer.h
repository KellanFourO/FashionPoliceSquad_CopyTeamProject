#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit								 CLayer();
	explicit								 CLayer(LAYERTAG _eLayerTag);
	virtual									~CLayer();

public:
	//Get
	CComponent*								Get_Component(COMPONENTID eID,OBJECTTAG eObjTag, COMPONENTTAG eComponentTag);
	vector<CGameObject*>&					Get_ObjectList(OBJECTTAG eObjTag) { return m_mapObject[eObjTag]; }
	map<OBJECTTAG, vector<CGameObject*>>&	Get_mapObject() { return m_mapObject; }

public:
	HRESULT									Add_GameObject(OBJECTTAG eObjTag, CGameObject* pGameObject);
	HRESULT									Delete_GameObject(OBJECTTAG eObjTag, CGameObject* pGameObject, _uint pIndex); //cube 등 오브젝트 삭제용
	HRESULT									Delete_GameObject2(OBJECTTAG eObjTag, CGameObject* pGameObject); //오브젝트 삭제용

public:
	HRESULT									Ready_Layer();
	_int									Update_Layer(const _float& fTimeDelta);
	void									LateUpdate_Layer();

private:
	CGameObject*							Find_GameObject(OBJECTTAG eObjTag);

private:
	map<OBJECTTAG, vector<CGameObject*>>	m_mapObject;
	LAYERTAG								m_eLayerTag;

public:
	static CLayer*							Create(LAYERTAG _eLayerTag);
	static CLayer*							Create();


private:
	virtual void							Free();
};

END