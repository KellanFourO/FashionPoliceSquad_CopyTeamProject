
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
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::CALCULATOR, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Build_Cube_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::COLLIDER, pComponent); // 승용 DYNAMIC에서 STATIC 변경

	return S_OK;


}


HRESULT CBuild_Obj::Ready_GameObject(_uint pRotate, _vec3 pMouse_Pos, _vec3 Size)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::BUILD_OBJ);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	m_pTransformCom->Set_Scale(Size);
	if (pRotate != 0)
	{
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(pRotate * 90.f));
	}
	m_pTransformCom->Set_Pos(pMouse_Pos);

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], m_pTransformCom->m_vScale);

	return S_OK;
}


_int CBuild_Obj::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)
	{
		Engine::Add_RenderGroup(RENDER_BLOCK, this);
	}
	else if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
	{
		Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	}

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
	
	m_pCollider->Render_Collider(); //


	if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)  //텍스쳐가 버퍼보다 먼저 ... ... ㅎ... ...
	{	
		m_VecTempCube = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecTempCube();
		m_pTextureCom->Render_OBJTextrue(m_VecTempCube[m_TextureNumber - cubeTextureStartIndex]);
		m_pBufferCubeCom->Render_Buffer();  
	}
	else if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
	{
		m_VecTempPlane = dynamic_cast<CMapTool*>(Engine::Management()->Get_One_Scene(SCENETAG::MAPTOOL))->Get_VecTempPlane();
		m_pTextureCom->Render_OBJTextrue(m_VecTempPlane[m_TextureNumber - planeTextureStartIndex]);
		m_pBufferRcCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
 

CBuild_Obj* CBuild_Obj::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, 
	_uint TextureNum, _vec3 Size, _uint pRotate, _uint Index, OBJ_TYPE eType)
{
	CBuild_Obj* pInstance = new CBuild_Obj(pGraphicDev);

	pInstance->Set_OBJ_TYPE(eType);
	pInstance->Set_OBJIndex(Index);
	pInstance->Set_TextrureNum(TextureNum);
	
	if (FAILED(pInstance->Ready_GameObject(pRotate, pMouse_Pos, Size)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Build_OBJ Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBuild_Obj::Free()
{
	__super::Free();
}
