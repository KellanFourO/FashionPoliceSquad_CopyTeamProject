#include "stdafx.h"
#include "MuzzleFlash.h"
#include "PaintBall.h"
#include "MuzzleFlash.h"
#include "Dyehard.h"

#include "Export_System.h"
#include "Export_Utility.h"

CMuzzleFlash::CMuzzleFlash(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CMuzzleFlash::CMuzzleFlash(const CMuzzleFlash& rhs)
	: Engine::CGameObject(rhs)
{
}

CMuzzleFlash::~CMuzzleFlash()
{
}

HRESULT CMuzzleFlash::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

		return S_OK;
}

Engine::_int CMuzzleFlash::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	CTransform * pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	NULL_CHECK_RETURN(pPlayerTransCom, -1); // 플레이어 가져오기

	_vec3	vPlayerPos, vPlayerLook, vPlayerUp;

	pPlayerTransCom->Get_Info(INFO_UP, &vPlayerUp);
	pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	pPlayerTransCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_vec3 vGunMove;

	D3DXVec3Cross(&vGunMove, &vPlayerUp, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	D3DXVec3Normalize(&vGunMove, &vGunMove);


	_vec3 vGunScale = { 0.11f,0.11f,0.11f };

	_vec3 vGunMoveRight = vGunMove / 20;
	_vec3 vGunMoveDown = -vPlayerUp / 20;

	m_fGunMoveRight = 1.6;
	m_fGunMoveDown = 1.1;

	m_pTransformCom->Set_Scale(vGunScale);
	m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook / 4 + vGunMoveRight * m_fGunMoveRight + vGunMoveDown * m_fGunMoveDown);

	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_matrix		matRot;
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_matrix		matRot;
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
	}

	m_pTransformCom->Get_Info(INFO_POS, &dynamic_cast<CGun*>(m_pGun)->m_vShotPos);


	__super::Update_GameObject(fTimeDelta);

	

	return OBJ_NOEVENT;
}

void CMuzzleFlash::LateUpdate_GameObject()
{
}

void CMuzzleFlash::Render_GameObject()
{
	if (dynamic_cast<CDyehard*>(m_pGun)->m_bFire) {
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

HRESULT CMuzzleFlash::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleFlash"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

CMuzzleFlash* CMuzzleFlash::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMuzzleFlash* pInstance = new CMuzzleFlash(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CMuzzleFlash::Free()
{
	__super::Free();
}
