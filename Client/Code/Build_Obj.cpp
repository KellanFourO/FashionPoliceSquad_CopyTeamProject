
#include "stdafx.h"
#include "Build_Obj.h"
#include "ImGuiManager.h"

#include "MapTool.h"
#include "Stage.h"

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

	if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubeTexture"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);
	}
	if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlaneTexture"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);
	}

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

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
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(pRotate * 45.f));
	}
	m_pTransformCom->Set_Pos(pMouse_Pos);
	m_myObjPos = pMouse_Pos;
	
	if (m_eAttribute == OBJ_ATTRIBUTE::DES_OBJ) //속성-파괴일 경우
	{
		m_iHp = 30;  
		m_eInteraction = OBJ_INTERACTION::INTER_NONE;
	}
	else if (m_eAttribute == OBJ_ATTRIBUTE::INTER_OBJ) //속성-상호작용일 경우
	{
		m_iHp = 99999999;
		m_eInteraction = OBJ_INTERACTION::OBJ_LOCK_OFF; 
		//걸린(엮인?) 순간에만 LOCK_ON, 그 외의 순간엔 LOCK_OFF로 쓰게
	}
	else //속성 NONE이거나 LIGHT인 경우
	{
		m_iHp = 99999999;
		m_eInteraction = OBJ_INTERACTION::INTER_NONE;
	}

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], m_pTransformCom->m_vScale);
	
	// billboard 용
	m_pPlayerTransform = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	return S_OK;
}


_int CBuild_Obj::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)
	{
		Engine::Add_RenderGroup(RENDER_ALPHATEST, this); //Render_Block은 Octree 용
	}
	else if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
	{
		Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

		if (m_eAttribute == OBJ_ATTRIBUTE::BILL_OBJ)
	 	{
			Init_PlayerTransform();
			if (m_pPlayerTransform && m_bBillBoard)
			{
				BillBoard();
			}
		}
		if (m_eAttribute == OBJ_ATTRIBUTE::INTER_OBJ)
		{
			Init_PlayerTransform();
			if (m_pPlayerTransform && m_bBillBoard)
			{
				BillBoard_X();
			}
		}
	}

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CBuild_Obj::LateUpdate_GameObject()
{
	auto& mapSceneTemp = Engine::Management()->Get_MapScene();
	for (auto& iter : mapSceneTemp) { m_NowScene = iter.first; }

	__super::LateUpdate_GameObject();
}

void CBuild_Obj::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pCollider->Render_Collider(); //
	FAILED_CHECK_RETURN(SetUp_Meterial(), );
	
	Render_Texture();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}
 

void CBuild_Obj::Render_Texture()
{
	if (m_NowScene == SCENETAG::MAPTOOL)
	{
		if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)  //텍스쳐가 버퍼보다 먼저 ... ... ㅎ... ...
		{
			m_pTextureCom->Render_ObjCubeTex(m_TextureNumber);
			m_pBufferCubeCom->Render_Buffer();
		}
		else if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
		{
			m_pTextureCom->Render_ObjPlaneTex(m_TextureNumber);
			m_pBufferRcCom->Render_Buffer();
		}
	}

	if (m_NowScene == SCENETAG::STAGE)
	{
		if (m_eOBJ_Type == OBJ_TYPE::CUBE_OBJ)
		{
			m_pTextureCom->Render_ObjCubeTex(m_TextureNumber);
			m_pBufferCubeCom->Render_Buffer();
		}
		else if (m_eOBJ_Type == OBJ_TYPE::PLANE_OBJ)
		{
			m_pTextureCom->Render_ObjPlaneTex(m_TextureNumber);
			m_pBufferRcCom->Render_Buffer();
		}
	}

	//스테이지 변경에 따른 반영사항 추가 필요
}

HRESULT CBuild_Obj::SetUp_Meterial()
{
	D3DMATERIAL9 tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Ambient = { 1.f, 1.f, 1.f, 1.0f };  //마지막 값 -> 알파채널(투명도)
	tMtrl.Emissive = { 0.1f, 0.1f, 0.1f, 0.1f };
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}


void CBuild_Obj::BillBoard()
{
	_vec3 vPlayerPos, vPlayerPos_Rel;
	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos.y = 0.f;
	// 이동 코드

	m_pTransformCom->Get_Info(INFO_POS, &m_myObjPos);
	vPlayerPos_Rel = vPlayerPos - m_myObjPos;

	D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);

	_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
	//방향전환 코드 (플레이어 방향)
}

void CBuild_Obj::BillBoard_X()
{
 	_vec3 vPlayerPos, vPlayerPos_Rel;
 	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
 	vPlayerPos.x = 0.f;
 	// 이동 코드
 
 	m_pTransformCom->Get_Info(INFO_POS, &m_myObjPos);
 	vPlayerPos_Rel = vPlayerPos - m_myObjPos;
 
 	D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
 
 	_float fAngle = atan2f(vPlayerPos_Rel.y, vPlayerPos_Rel.z);
 	m_pTransformCom->Set_Rotate(ROT_X, fAngle + D3DX_PI);
 	//방향전환 코드 (플레이어 방향)
}

void CBuild_Obj::Init_PlayerTransform()
{
	if (m_bLateInit)
	{
		m_pPlayerTransform = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		m_bLateInit = false;
	}
}

CBuild_Obj* CBuild_Obj::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, 
	_uint TextureNum, _vec3 Size, _uint pRotate, _uint Index, OBJ_TYPE eType, OBJ_ATTRIBUTE eAttri)
{
	CBuild_Obj* pInstance = new CBuild_Obj(pGraphicDev);

	pInstance->Set_OBJ_TYPE(eType);
	pInstance->Set_OBJ_ATTRIBUTE(eAttri);
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
