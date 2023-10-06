#include "stdafx.h"
#include "PaintShotGun.h"
#include "Player.h"

#include "PaintShotGun_Enter.h"
#include "PaintShotGun_Idle.h"
#include "PaintShotGun_Jump.h"
#include "PaintShotGun_Lazer.h"
#include "PaintShotGun_Ready.h"
#include "PaintShotGun_Reload.h"
#include "PaintShotGun_Shot.h"
#include "PaintShotGun_Walk.h"


#include "Export_System.h"
#include "Export_Utility.h"

CPaintShotGun::CPaintShotGun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPlayerGun(pGraphicDev)
{
}

CPaintShotGun::CPaintShotGun(const CPaintShotGun& rhs)
	: CPlayerGun(rhs)
{
}

CPaintShotGun::~CPaintShotGun()
{
}

HRESULT CPaintShotGun::Ready_GameObject()
{
	__super::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	ReadyState();

	m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(3.f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(3.f));
	m_eBulletType = BULLETTYPE::SHOTGUN_BULLET;

	m_pGunState = m_pStateArray[IDLE];
	m_pGunState->Initialize(this);

	m_fGunMoveRight = 3;
	m_fGunMoveDown = 1.f;
	m_vScale = { 1.3f,1.3f,1.3f };

	m_fFireSpeed = 1.f;
	m_fReloadSpeed = 1.f;

	m_iBulletColor = 0;
	m_iCurrentBullet = 7;
	m_iReloadBullet = m_iCurrentBullet * 1;
	m_iMaxBullet = m_iReloadBullet * 15;

	m_bLazer = false;
	m_bFire = false;
	m_bCharged = false;

	m_pTransformCom->Set_Scale(m_vScale);

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-dyehard-muzlflash2.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-dyehard-charged2.png", 2);
	return S_OK;
}

Engine::_int CPaintShotGun::Update_GameObject(const _float& fTimeDelta)
{
	HostMove(fTimeDelta);

	if (dynamic_cast<CPlayer*>(m_pHost)->Get_INFO()->Player_GunType == PLAYER_GUNTYPE::SHOTGUN)
	{
		__super::Update_GameObject(fTimeDelta);
	}
	return OBJ_NOEVENT;
}

void CPaintShotGun::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CPaintShotGun::Render_GameObject()
{

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CPaintShotGun::Add_Component()
{

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Dyehard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}


void CPaintShotGun::ReadyState()
{
	m_pStateArray[IDLE]		= new CPaintShotGun_Idle;
	m_pStateArray[WALK]		= new CPaintShotGun_Walk;
	m_pStateArray[JUMP]		= new CPaintShotGun_Jump;
	m_pStateArray[READY]	= new CPaintShotGun_Ready;
	m_pStateArray[SHOT]		= new CPaintShotGun_Shot;
	m_pStateArray[RELOAD]	= new CPaintShotGun_Reload;
	m_pStateArray[LAZER]	= new CPaintShotGun_Lazer;
	m_pStateArray[ENTER]	= new CPaintShotGun_Enter;
}

CPaintShotGun* CPaintShotGun::Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer)
{
	CPaintShotGun* pInstance = new CPaintShotGun(pGraphicDev);
	pInstance->m_pHost = pPlayer;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PaintShotGun Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CPaintShotGun::Free()
{
	for (auto iter : m_pStateArray)
		Safe_Delete(iter);

	__super::Free();

}
