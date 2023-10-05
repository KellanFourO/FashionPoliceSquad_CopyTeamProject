
#include "stdafx.h"
#include "Build_Obj.h"
#include "ImGuiManager.h"
#include "MapTool.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBuild_Obj::CBuild_Obj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBuild_Obj::CBuild_Obj(const CBuild_Obj& rhs)
	: CGameObject(rhs)
{
}

CBuild_Obj::~CBuild_Obj()
{
}


HRESULT CBuild_Obj::Add_Component()
{
	CComponent* pComponent = nullptr;

	switch (m_eOBJ_Type)
	{
	case OBJ_TYPE::CUBE_OBJ:
		pComponent = m_pBufferCubeCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);
		break;

	case OBJ_TYPE::CROSS_OBJ:
		pComponent = m_pBufferCrossCom = dynamic_cast<CCrossTex*>(Engine::Clone_Proto(L"Proto_CrossTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);
		break;

	case OBJ_TYPE::PLANE_OBJ:
		pComponent = m_pBufferRcCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_PlaneTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);
		break;

	default:
		break;
	}

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::CALCULATOR, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Build_Cube_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;


}


HRESULT CBuild_Obj::Ready_GameObject(_vec3 pMouse_Pos, _vec3 Size)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::BUILD_OBJ);


	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	m_pTransformCom->Set_Scale(Size);
	m_pTransformCom->Set_Pos(pMouse_Pos);

	return S_OK;
}

_int CBuild_Obj::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CBuild_Obj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBuild_Obj::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)
	{	
		m_VecTempCube = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecTempCube();
		m_pBufferCubeCom->Render_Buffer();  
		m_pTextureCom->Render_OBJTextrue(m_VecTempCube[m_TextureNumber - cubeTextureStartIndex]);
	}
	else if (m_eOBJ_Type == OBJ_TYPE::CROSS_OBJ)
	{
		m_VecTempCross = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecTempCross();
		m_pBufferCrossCom->Render_Buffer();
		m_pTextureCom->Render_OBJTextrue(m_VecTempCross[m_TextureNumber - crossTextureStartIndex]);
	}
	else if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
	{
		m_VecTempPlane = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecTempPlane();
		m_pBufferRcCom->Render_Buffer();
		m_pTextureCom->Render_OBJTextrue(m_VecTempPlane[m_TextureNumber - planeTextureStartIndex]);
	}
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
 

CBuild_Obj* CBuild_Obj::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, _uint TextureNum, _vec3 Size, _uint Index, OBJ_TYPE eType)
{
	CBuild_Obj* pInstance = new CBuild_Obj(pGraphicDev);

	pInstance->Set_OBJ_TYPE(eType);
	pInstance->Set_OBJIndex(Index);
	pInstance->Set_TextrureNum(TextureNum);
	
	if (FAILED(pInstance->Ready_GameObject(pMouse_Pos, Size)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Build_Cube Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBuild_Obj::Free()
{
	__super::Free();
}
