#include "stdafx.h"
#include "MuzzlefFlash_Rifle.h"
#include "TailorAssertRifle.h"

#include "Export_System.h"
#include "Export_Utility.h"

CMuzzleFlash_Rifle::CMuzzleFlash_Rifle(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev), m_RandAngle(0)
{
}

CMuzzleFlash_Rifle::CMuzzleFlash_Rifle(const CMuzzleFlash_Rifle& rhs)
	: Engine::CGameObject(rhs), m_RandAngle(0)
{
}

CMuzzleFlash_Rifle::~CMuzzleFlash_Rifle()
{
}

HRESULT CMuzzleFlash_Rifle::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vGunScale = { 0.08f,0.08f,0.08f };
	m_pTransformCom->Set_Scale(vGunScale);


	return S_OK;
}

Engine::_int CMuzzleFlash_Rifle::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	if (m_bLateInit)
	{
		m_pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		m_pPlayerGun = dynamic_cast<CTailorAssertRifle*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER_GUN).back());
		dynamic_cast<CTailorAssertRifle*>(m_pPlayerGun)->Set_Flash(this);


		m_bLateInit = false;
	}



	_vec3	vPlayerPos, vPlayerLook, vPlayerUp;

	m_pPlayerTransform->Get_Info(INFO_UP, &vPlayerUp);
	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_vec3 vGunMove;

	D3DXVec3Cross(&vGunMove, &vPlayerUp, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	D3DXVec3Normalize(&vGunMove, &vGunMove);



	_vec3 vGunMoveRight = vGunMove / 20;
	_vec3 vGunMoveDown = -vPlayerUp / 20;

	m_fGunMoveRight = 1.36;
	m_fGunMoveDown = 0.68;

	m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook / 4 + vGunMoveRight * m_fGunMoveRight + vGunMoveDown * m_fGunMoveDown);


	if (m_bFire)
	{
		m_fRenderingTick += fTimeDelta;

		if (m_fRenderingTick > 0.1f)
		{
			m_RandAngle -= rand() % 20;
			m_bFire = false;
			dynamic_cast<CTailorAssertRifle*>(m_pPlayerGun)->Set_Index(0);
			m_fRenderingTick = 0.f;
		}

	}
	else
	{
		m_pTransformCom->RotateAxis(vPlayerLook, m_RandAngle);
	}

	Mouse_Input();



	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CMuzzleFlash_Rifle::LateUpdate_GameObject()
{

}

void CMuzzleFlash_Rifle::Render_GameObject()
{
	if (m_bFire)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CMuzzleFlash_Rifle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleFlash2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMuzzleFlash_Rifle::Mouse_Input()
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

CMuzzleFlash_Rifle* CMuzzleFlash_Rifle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMuzzleFlash_Rifle* pInstance = new CMuzzleFlash_Rifle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MuzzleFlash_Rifle Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CMuzzleFlash_Rifle::Free()
{
	__super::Free();
}
