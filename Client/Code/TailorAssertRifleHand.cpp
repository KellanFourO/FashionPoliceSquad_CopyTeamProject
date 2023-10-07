#include "stdafx.h"
#include "TailorAssertRifleHand.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

CTailorAssertRifleHand::CTailorAssertRifleHand(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTailorAssertRifleHand::CTailorAssertRifleHand(const CTailorAssertRifleHand& rhs)
	:CGameObject(rhs)
{
}

CTailorAssertRifleHand::~CTailorAssertRifleHand()
{
}

HRESULT CTailorAssertRifleHand::Ready_GameObject()
{
	__super::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(3.f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(3.f));

	m_fGunMoveRight = 3;
	m_fGunMoveDown = 1.f;
	m_vScale = { 0.5f,0.5f,0.5f };
	m_pTransformCom->Set_Scale(m_vScale);

	return S_OK;
}

Engine::_int CTailorAssertRifleHand::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bLateInit)
	{
		m_pRifle = dynamic_cast<CTailorAssertRifle*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER_GUN).back());
		m_pHost = dynamic_cast<CPlayer*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER).back());
		m_bLateInit = false;
	}

	m_fGunMoveDown = m_pRifle->Get_GunMoveDown();
	m_fGunMoveRight = m_pRifle->Get_GunMoveRight();
	m_vScale = m_pRifle->Get_Scale();

	//TODO 총이 플레이어와 상호작용하여 항상 일정한 위치 유지
	_vec3 vPlayerPos, vPlayerLook, vPlayerUp;

	m_pHost->Get_Transform()->Get_Info(INFO_UP, &vPlayerUp);
	m_pHost->Get_Transform()->Get_Info(INFO_POS, &vPlayerPos);
	m_pHost->Get_Transform()->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_vec3 vGunMove;

	D3DXVec3Cross(&vGunMove, &vPlayerUp, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	D3DXVec3Normalize(&vGunMove, &vGunMove);

	_vec3 vGunMoveRight = vGunMove / 20;
	_vec3 vGunMoveDown = -vPlayerUp / 20;

	_vec3 vPos = { vPlayerPos + vPlayerLook * 2.5f + vGunMoveRight * m_fGunMoveRight * 3.7f + vGunMoveDown * m_fGunMoveDown * 22.5f };

	m_pTransformCom->Set_Pos(vPos);

	Mouse_Input();

	if (m_pHost->Get_INFO()->Player_GunType == PLAYER_GUNTYPE::ASSERTRIFLE)
	{
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		__super::Update_GameObject(fTimeDelta);
	}

 	return OBJ_NOEVENT;
}

void CTailorAssertRifleHand::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CTailorAssertRifleHand::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}


HRESULT CTailorAssertRifleHand::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RifleHand"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CTailorAssertRifleHand::Mouse_Input()
{
	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
	}
}

CTailorAssertRifleHand* CTailorAssertRifleHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTailorAssertRifleHand* pInstance = new CTailorAssertRifleHand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("TailorAssertRifleHand Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CTailorAssertRifleHand::Free()
{
	__super::Free();
}
