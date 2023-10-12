#include "stdafx.h"
#include "UI_MissionObjective.h"

#include "Export_Utility.h"
#include "Export_System.h"


CMissionObjective::CMissionObjective(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CMissionObjective::CMissionObjective(const CMissionObjective& rhs)
	: Engine::CGameObject(rhs)
{
}

CMissionObjective::~CMissionObjective()
{
}

HRESULT Engine::CMissionObjective::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fRatio = 1.2f;

	m_vPos = { 150.f, 50.f, 0.f };
	m_vScale = { 128.f * fRatio, 32.f * fRatio, 1.f };

	m_vPos.x = m_vPos.x - WINCX * 0.5f; // 150 - 400 = -250
	m_vPos.y = -m_vPos.y + WINCY * 0.5f; // -50 + 300 = 250


	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	m_wstrObjective = L"";
	m_wstrTitle = L"";

	return S_OK;
}

Engine::_int Engine::CMissionObjective::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);

	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CMissionObjective::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMissionObjective::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);


	m_pTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();

	if (m_wstrObjective != L"")
	{
		wstring wstrTempObjective = L" ¡ß " + m_wstrObjective;

		Engine::Render_Font(L"MISSION_FONT", m_wstrTitle.c_str(), &_vec2(20, 20), D3DXCOLOR(D3DCOLOR_ARGB(255, 254, 214, 147)));
		Engine::Render_Font(L"MISSION_FONT", wstrTempObjective.c_str(), &_vec2(20, 60), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)));
	}


	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CMissionObjective::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MissionUITexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CMissionObjective* CMissionObjective::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissionObjective* pInstance = new CMissionObjective(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MissionObjective Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CMissionObjective::Free()
{
	__super::Free();
}
