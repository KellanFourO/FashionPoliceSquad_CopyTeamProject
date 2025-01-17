#include "stdafx.h"
#include "..\Header\SkyBox.h"

#include "Export_System.h"
#include "Export_Utility.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	: Engine::CGameObject(rhs)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SkyBoxTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkyBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();
}

HRESULT CSkyBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 400.f, 400.f, 400.f };

	return S_OK;
}

Engine::_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);
	__super::Update_GameObject(fTimeDelta);


	return	OBJ_NOEVENT;
}

void CSkyBox::LateUpdate_GameObject()
{
	_matrix	matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, nullptr, &matCamWorld);

	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);
}
void CSkyBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSkyBox::Render_Texture()
{
	auto& mapSceneTemp = Engine::Management()->Get_MapScene();
	for (auto& iter : mapSceneTemp) 
	{ 
		if (iter.second == Engine::Management()->Get_Scene())
		m_NowScene = iter.first; 
	}
		
	if ((m_NowScene == SCENETAG::STAGE) || (m_NowScene == SCENETAG::LOBBY))
	{
		m_pTextureCom->Render_Textrue(0);
	}
	else if (m_NowScene == SCENETAG::BOSS_STAGE)
	{
		m_pTextureCom->Render_Textrue(1);
	}
	else if (m_NowScene == SCENETAG::STAGE2)
	{
		m_pTextureCom->Render_Textrue(2);
	}


}



