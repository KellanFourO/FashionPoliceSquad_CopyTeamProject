#include "stdafx.h"
#include "UI_PlayerFace.h"
#include "UIMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include "PlayerState.h"

CPlayerFace::CPlayerFace(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CPlayerFace::CPlayerFace(const CPlayerFace& rhs)
	: Engine::CGameObject(rhs)
{
}

CPlayerFace::~CPlayerFace()
{
}

HRESULT Engine::CPlayerFace::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPos = { 50.f, 560.f, 0.f };
	m_vScale = { 28.f, 35.f, 1.f };

	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Player_HUD/des-hud_left.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Player_HUD/des-hud_right.png", 2);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Player_HUD/des-hud_front_hurt.png", 3);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Player_HUD/des-hud_smile.png", 4);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Player_HUD/des-hud_smirk_low.png", 5);


	m_pPlayer = Management()->Get_Player();

	return S_OK;
}

Engine::_int Engine::CPlayerFace::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);

	m_pTransformCom->m_vScale.x = m_vScale.x;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_fY;

	switch (m_pPlayer->Get_INFO()->PlayerState->StateID)
	{
	case PlayerStateID::Player_Left:
	{
		m_iTextureIndex = 1;
		break;
	}

	case PlayerStateID::Player_Right:
	{
		m_iTextureIndex = 2;
		break;
	}

	case PlayerStateID::Player_Hit:
	{
		m_iTextureIndex = 3;
		break;
	}

	case PlayerStateID::Player_Heal:
	{
		m_iTextureIndex = 4;
		break;
	}

	case PlayerStateID::Player_Armor:
	{
		m_iTextureIndex = 5;
		break;
	}

	case PlayerStateID::Player_Lv_Up:
	{
		m_iTextureIndex = 4;
		break;
	}

	case PlayerStateID::Player_IDLE:
	{
		m_iTextureIndex = 0;
		break;
	}
	}

	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CPlayerFace::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CPlayerFace::Render_GameObject()
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

HRESULT Engine::CPlayerFace::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerFaceTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CPlayerFace* CPlayerFace::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerFace* pInstance = new CPlayerFace(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("PlayerFace Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CPlayerFace::Free()
{
	__super::Free();
}
