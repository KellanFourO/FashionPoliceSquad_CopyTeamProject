
#include "stdafx.h"
#include "../Header/Build_Cube.h"
#include "ImGuiManager.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBuild_Cube::CBuild_Cube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBuild_Cube::CBuild_Cube(const CBuild_Cube& rhs)
	: CGameObject(rhs)
{
}

CBuild_Cube::~CBuild_Cube()
{
}

HRESULT CBuild_Cube::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Build_Cube_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::COLLIDER, pComponent); // 승용 DYNAMIC에서 STATIC 변경

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


HRESULT CBuild_Cube::Ready_GameObject(_vec3 pMouse_Pos, _vec3 Size)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::BUILD_CUBE);


	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	m_pTransformCom->Set_Pos(pMouse_Pos);
	m_pTransformCom->Set_Scale(Size);

// 	if (pMouse_Pos.y >= 40.f) {
// 		Set_ObjectTag(OBJECTTAG::DEAD_OBJ);
// 	}
// 	else {
// 		Set_ObjectTag(OBJECTTAG::BUILD_CUBE);
//
// 	}


	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], m_pTransformCom->m_vScale);
	return S_OK;
}

_int CBuild_Cube::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_BLOCK, this);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);


	__super::Update_GameObject(fTimeDelta);


	return 0;

}

void CBuild_Cube::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBuild_Cube::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//m_pCollider->Render_Collider(); //

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	FAILED_CHECK_RETURN(SetUp_Meterial(), );

	m_pTextureCom->Render_Textrue(m_TextureNumber);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}


HRESULT CBuild_Cube::SetUp_Meterial()
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

void CBuild_Cube::OnCollisionEnter(CCollider* _pOther)
{
}

void CBuild_Cube::OnCollisionStay(CCollider* _pOther)
{
}

void CBuild_Cube::OnCollisionExit(CCollider* _pOther)
{
}

CBuild_Cube* CBuild_Cube::Create
(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pMouse_Pos, _uint TextureNum, _vec3 CubeSize, _uint CubeIndex)
{
	CBuild_Cube* pInstance = new CBuild_Cube(pGraphicDev);
	pInstance->m_TextureNumber = TextureNum;
	pInstance->Set_CubeIndex(CubeIndex);

	if (FAILED(pInstance->Ready_GameObject(pMouse_Pos, CubeSize)))

	{
		Safe_Release(pInstance);

		MSG_BOX("Build_Cube Create Failed");
		return nullptr;
	}

	//int i = 0;
	return pInstance;
}

void CBuild_Cube::Free()
{
	__super::Free(); // 승용 SafeRelase에서 터지길래 잠시막아놓음
}