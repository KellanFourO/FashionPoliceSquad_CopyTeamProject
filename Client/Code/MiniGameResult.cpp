#include "stdafx.h"
#include "MiniGameResult.h"

#include "Export_System.h"
#include "Export_Utility.h"

CMiniGameResult::CMiniGameResult(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMiniGameResult::CMiniGameResult(const CMiniGameResult& rhs)
	: Engine::CGameObject(rhs)
{
}

CMiniGameResult::~CMiniGameResult()
{
}

HRESULT CMiniGameResult::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;
	m_fSize = 80.f;

	vPos = { 400.f, 300.f, 0.f };
	vScale = { m_fSize * fMultiply, m_fSize * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}

HRESULT CMiniGameResult::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Result_Hand"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

_int CMiniGameResult::Update_GameObject(const _float& fTimeDelta)
{
	if (m_iCallTime > 0) {

		Engine::Add_RenderGroup(RENDER_MINIGAME, this);
		int iExit = __super::Update_GameObject(fTimeDelta);
		--m_iCallTime;
	}

	return 0;
}

void CMiniGameResult::LateUpdate_GameObject()
{
	if (m_iCallTime > 0) {

		__super::LateUpdate_GameObject();

	}
}

void CMiniGameResult::Render_GameObject()
{
	if (m_iCallTime > 0) {
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
}

void CMiniGameResult::Call(_int _Call_Time, _int TexNum)
{
	Set_CallTime(_Call_Time);
	Set_Texture(TexNum);
}


CMiniGameResult* CMiniGameResult::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniGameResult* pInstance = new CMiniGameResult(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Player Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMiniGameResult::Free()
{
	__super::Free();
}
