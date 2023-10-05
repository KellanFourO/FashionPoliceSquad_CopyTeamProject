#include "stdafx.h"
#include "TailorAssertRifle.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

CTailorAssertRifle::CTailorAssertRifle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPlayerGun(pGraphicDev)
{
}

CTailorAssertRifle::CTailorAssertRifle(const CTailorAssertRifle& rhs)
	: CPlayerGun(rhs)
{
}

CTailorAssertRifle::~CTailorAssertRifle()
{
}

HRESULT CTailorAssertRifle::Ready_GameObject()
{
	__super::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	ReadyState();

	m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(3.f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(3.f));
	m_eBulletType = BULLETTYPE::ASSERTRIFLE_BULLET;

	m_fGunMoveRight = 3;
	m_fGunMoveDown = 1.f;
	m_vScale = { 1.3f,1.3f,1.3f };

	m_pTransformCom->Set_Scale(m_vScale);

	return S_OK;
}

Engine::_int CTailorAssertRifle::Update_GameObject(const _float& fTimeDelta)
{
	if (dynamic_cast<CPlayer*>(m_pHost)->Get_INFO()->Player_GunType == PLAYER_GUNTYPE::ASSERTRIFLE)
	{
		HostMove(fTimeDelta);

		__super::Update_GameObject(fTimeDelta);
	}

 	return OBJ_NOEVENT;
}

void CTailorAssertRifle::LateUpdate_GameObject()
{

}

void CTailorAssertRifle::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CTailorAssertRifle::ReadyState()
{
}

HRESULT CTailorAssertRifle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

CTailorAssertRifle* CTailorAssertRifle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer)
{
	CTailorAssertRifle* pInstance = new CTailorAssertRifle(pGraphicDev);
	pInstance->m_pHost = pPlayer;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("TailorAssertRifle Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CTailorAssertRifle::Free()
{

	__super::Free();
}
