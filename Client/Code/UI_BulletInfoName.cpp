#include "stdafx.h"
#include "UI_BulletInfoName.h"
#include "FontMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"



CBulletInfoName::CBulletInfoName(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CBulletInfoName::CBulletInfoName(const CBulletInfoName& rhs)
	: Engine::CGameObject(rhs)
{
}

CBulletInfoName::~CBulletInfoName()
{
}

HRESULT Engine::CBulletInfoName::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_vPos = { 715.f, 525.f, 0.f };
	m_vScale = { 85.f, 15.f, 1.f };

	m_fX = m_vPos.x - WINCX * 0.5f; // 150 - 400 = -250
	m_fY = -m_vPos.y + WINCY * 0.5f; // -50 + 300 = 250

	m_pTransformCom->m_vScale.x = m_vScale.x;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_fY;


	m_pPlayer = Management()->Get_Player();
	return S_OK;
}

Engine::_int Engine::CBulletInfoName::Update_GameObject(const _float& fTimeDelta)
{
// 	if (m_bLateInit)
// 	{
// 		m_pPlayer = dynamic_cast<CPlayer*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back());
// 		m_bLateInit = false;
// 	}




	Engine::Add_RenderGroup(RENDER_UI, this);



	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CBulletInfoName::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBulletInfoName::Render_GameObject()
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
		Engine::Render_Font(L"UI_WEAPON_NAME", m_pPlayer->Get_Gun()->Get_GunInfo()->m_szGunName, &_vec2(665, 520), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)), 20, false);
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CBulletInfoName::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BulletNameTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CBulletInfoName* CBulletInfoName::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBulletInfoName* pInstance = new CBulletInfoName(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("HPBar Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CBulletInfoName::Free()
{
	__super::Free();
}
