#include "stdafx.h"
#include "UI_WeaponIcon.h"

#include "Export_Utility.h"
#include "Export_System.h"



CWeaponIcon::CWeaponIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CWeaponIcon::CWeaponIcon(const CWeaponIcon& rhs)
	: Engine::CGameObject(rhs)
{
}

CWeaponIcon::~CWeaponIcon()
{
}

HRESULT Engine::CWeaponIcon::Ready_GameObject(UIDATA& _Info, _int _TextureIndex)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo = _Info;
	m_iTextureIndex = _TextureIndex;

	m_vPos = { m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z};
	m_vScale = { m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f };

	m_fX = m_vPos.x - WINCX * 0.5f; // 150 - 400 = -250
	m_fY = -m_vPos.y + WINCY * 0.5f; // -50 + 300 = 250

	m_pTransformCom->m_vScale.x = m_vScale.x;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_fY;


	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/WEAPON/ui-weapons_1.png", 1);

	return S_OK;
}

Engine::_int Engine::CWeaponIcon::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bSelect)
	{
		Engine::Add_RenderGroup(RENDER_UI, this);
		_int iExit = __super::Update_GameObject(fTimeDelta);
	}



	return 0;
}

void Engine::CWeaponIcon::LateUpdate_GameObject()
{

	__super::LateUpdate_GameObject();
}

void CWeaponIcon::Render_GameObject()
{
	if (m_bSelect)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


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

HRESULT Engine::CWeaponIcon::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunIconTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CWeaponIcon* CWeaponIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, UIDATA& _Info, _int _TextureIndex)
{
	CWeaponIcon* pInstance = new CWeaponIcon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_Info,_TextureIndex)))
	{
		Safe_Release(pInstance);

		MSG_BOX("WeaponIcon Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CWeaponIcon::Free()
{
	__super::Free();
}
