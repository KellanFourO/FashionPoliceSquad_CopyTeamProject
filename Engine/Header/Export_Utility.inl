CManagement* Management()
{
	return CManagement::GetInstance();
}
CComponent* Get_Component(COMPONENTID eID, LAYERTAG eLayerTag, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag)
{
	return CManagement::GetInstance()->Get_Component(eID, eLayerTag, eObjTag, eComponentTag);
}

map<SCENETAG, CScene*> Get_MapScene()
{
	return CManagement::GetInstance()->m_mapScene;
}

void Set_MapScene(map<SCENETAG, CScene*> pMapScene)
{
	CManagement::GetInstance()->m_mapScene = pMapScene;
}

CScene* Get_One_Scene(SCENETAG eSceneTag)
{
	return CManagement::GetInstance()->m_mapScene[eSceneTag];
}


HRESULT      Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementInstance)
{
	CManagement* pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagementInstance = pManagement;

	return S_OK;
}

HRESULT      Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int         Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void         LateUpdate_Scene()
{
	CManagement::GetInstance()->LateUpdate_Scene();
}
void         Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

CProtoMgr* ProtoMgr()
{
    return CProtoMgr::GetInstance();
}

HRESULT         Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent* Clone_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

CRenderer* Renderer()
{
	return CRenderer::GetInstance();
}

void      Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eType, pGameObject);
}
void      Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void      Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

//Collision
CollisionMgr* CollisionManager()
{
	return CollisionMgr::GetInstance();
}

CSoundMgr* SoundMgr()
{
	return CSoundMgr::GetInstance();
}

COctree* Octree()
{
    return COctree::GetInstance();
}

void         Release_Utility()
{
	CollisionMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	COctree::GetInstance()->DestroyInstance();
}


//LightMgr
CLightMgr* LightMgr()
{
	return CLightMgr::GetInstance();
}

HRESULT    Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}