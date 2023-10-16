#include "stdafx.h"
#include "UI_BulletInfoCount.h"
#include "Export_Utility.h"
#include "Export_System.h"



CBulletInfoCount::CBulletInfoCount(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CBulletInfoCount::CBulletInfoCount(const CBulletInfoCount& rhs)
	: Engine::CGameObject(rhs)
{
}

CBulletInfoCount::~CBulletInfoCount()
{
}

HRESULT Engine::CBulletInfoCount::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_vPos = { 720.f, 570.f, 0.f };
	m_vScale = { 80.f, 20.f, 1.f };

	m_fX = m_vPos.x - WINCX * 0.5f; // 150 - 400 = -250
	m_fY = -m_vPos.y + WINCY * 0.5f; // -50 + 300 = 250

	m_pTransformCom->m_vScale.x = m_vScale.x;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_fY;

	m_pPlayer = Management()->Get_Player();
	return S_OK;
}

Engine::_int Engine::CBulletInfoCount::Update_GameObject(const _float& fTimeDelta)
{
// 	if (m_bLateInit)
// 	{
// 		m_pPlayer = dynamic_cast<CPlayer*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back());
// 		m_bLateInit = false;
//
// 	}




	Engine::Add_RenderGroup(RENDER_UI, this);



	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CBulletInfoCount::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBulletInfoCount::Render_GameObject()
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

	if (!m_pPlayer->Get_SceneChange())
	{
		const WCHAR* pInfiniteSymbol = L"¡Ä";
		Engine::Render_Font(L"UI_FONT", pInfiniteSymbol, &_vec2(670, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 130, 245, 209)), 20, false);
		Engine::Render_Font(L"UI_FONT", pInfiniteSymbol, &_vec2(730, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 130, 245, 209)), 20, false);
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CBulletInfoCount::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BulletCountTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CBulletInfoCount* CBulletInfoCount::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBulletInfoCount* pInstance = new CBulletInfoCount(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BulletCount Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CBulletInfoCount::Free()
{
	__super::Free();
}
