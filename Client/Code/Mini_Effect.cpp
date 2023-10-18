
#include "stdafx.h"
#include "Mini_Effect.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_Effect::CMini_Effect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_Effect::CMini_Effect(const CMini_Effect& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_Effect::~CMini_Effect()
{
}



HRESULT CMini_Effect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Number_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMini_Effect::Set_Move()
{
	m_pTransformCom->Set_Pos(m_PlayerPos);
}

void CMini_Effect::Set_Dir()
{
}

HRESULT CMini_Effect::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 400.f, 400.f, 0.f };
	vScale = { 30.f * fMultiply, 30.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_PlayerPos = vPos;
	m_PlayerScale = vScale;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}

_int CMini_Effect::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMini_Effect::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMini_Effect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}



CMini_Effect* CMini_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMini_Effect* pInstance = new CMini_Effect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Collider Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_Effect::Free()
{
	__super::Free();
}
