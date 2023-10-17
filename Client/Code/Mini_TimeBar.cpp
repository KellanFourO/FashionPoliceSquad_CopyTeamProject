
#include "stdafx.h"
#include "Mini_TimeBar.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_TimeBar::CMini_TimeBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_TimeBar::CMini_TimeBar(const CMini_TimeBar& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_TimeBar::~CMini_TimeBar()
{
}



HRESULT CMini_TimeBar::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_TimeBar_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

HRESULT CMini_TimeBar::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 200.f, 300.f, 0.f };
	vScale = { 15.f * fMultiply, 150.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_vStartPos = vPos;
	m_vStartScale = vScale;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}



void CMini_TimeBar::Time_Count()
{
	if ((m_fTimeCount > 0) && (m_iTextureIndex == 1)) {
		m_fTimeCount-= 0.25f;

		_vec3 vPos, vScale;

		vPos = { m_vStartPos.x, (m_vStartPos.y - (m_fTimeFullCount - m_fTimeCount)), m_vStartPos.z };
		vScale = { m_vStartScale.x, (m_vStartScale.y - (m_fTimeFullCount - m_fTimeCount)), m_vStartScale.z };

		m_pTransformCom->Set_Pos(vPos);
		m_pTransformCom->Set_Scale(vScale);
	}
}


_int CMini_TimeBar::Update_GameObject(const _float& fTimeDelta)
{
	Time_Count();

	Engine::Add_RenderGroup(RENDER_UI, this);
	int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMini_TimeBar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMini_TimeBar::Render_GameObject()
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



CMini_TimeBar* CMini_TimeBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, int m_iText)
{
	CMini_TimeBar* pInstance = new CMini_TimeBar(pGraphicDev);

	pInstance->Set_TextureNumber(m_iText);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Player Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_TimeBar::Free()
{
	__super::Free();
}
