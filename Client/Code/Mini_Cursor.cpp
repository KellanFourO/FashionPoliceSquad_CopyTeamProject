
#include "stdafx.h"
#include "Mini_Cursor.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_Cursor::CMini_Cursor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_Cursor::CMini_Cursor(const CMini_Cursor& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_Cursor::~CMini_Cursor()
{
}



HRESULT CMini_Cursor::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Cursor_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

HRESULT CMini_Cursor::Ready_GameObject(_vec3 pPos)
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vScale = { 26.f * fMultiply, 26.f * fMultiply, 1.f };

	m_pTransformCom->Set_Scale(vScale);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}


_int CMini_Cursor::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMini_Cursor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMini_Cursor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CMini_Cursor::Set_Pos_ArrowPoint(_vec3 ArrowPoint)
{
	m_pTransformCom->Set_Pos(ArrowPoint);
}

CMini_Cursor* CMini_Cursor::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pPos)
{
	CMini_Cursor* pInstance = new CMini_Cursor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Cursor Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_Cursor::Free()
{
	__super::Free();
}
