#pragma once
#include "Management.h"
#include "Scene.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "WireTex.h"
#include "UITex.h"
#include "SYTex.h"

#include "Texture.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"

#include "ProtoMgr.h"
#include "Renderer.h"
#include "CollisionMgr.h"

#include "CameraCom.h"

#include "LightMgr.h"
#include "Light.h"
#include "SoundMgr.h"
#include "OctreeNode.h"
#include "Octree.h"
#include "Frustum.h"

#include "RigidBody.h"
#include "PSystem.h"

BEGIN(Engine)
class CScene;
// Management
inline CManagement* Management();
inline CComponent* Get_Component(COMPONENTID eID, LAYERTAG eLayerTag, OBJECTTAG eObjTag, COMPONENTTAG eComponentTag);

inline map<SCENETAG, CScene*> Get_MapScene();
inline void			Set_MapScene(map<SCENETAG, CScene*> pMapScene);
inline CScene*		Get_One_Scene(SCENETAG eSceneTag);

inline HRESULT      Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementInstance);

inline HRESULT      Set_Scene(CScene* pScene);
inline _int         Update_Scene(const _float& fTimeDelta);
inline void         LateUpdate_Scene();
inline void         Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

// ProtoMgr
inline CProtoMgr*	ProtoMgr();
inline HRESULT      Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*	Clone_Proto(const _tchar* pProtoTag);

// Renderer
inline CRenderer* Renderer();
inline void      Add_RenderGroup(RENDERID eType, CGameObject* pGameObject);
inline void      Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void      Clear_RenderGroup();

// CollisionMgr
inline CollisionMgr* CollisionManager();

//LightMgr
inline CLightMgr* LightMgr();
inline HRESULT	  Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

//SoundMgr
inline CSoundMgr* SoundMgr();

//Octree
inline COctree* Octree();


inline void         Release_Utility();
#include "Export_Utility.inl"

END