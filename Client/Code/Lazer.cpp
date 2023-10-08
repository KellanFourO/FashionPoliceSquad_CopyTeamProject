#include "stdafx.h"
#include "Lazer.h"
#include "PaintShotGun.h"
#include "Export_System.h"
#include "Export_Utility.h"

CLazer::CLazer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CLazer::CLazer(const CLazer& rhs)
	: CBullet(rhs)
{
}

CLazer::~CLazer()
{
	Free();
}

HRESULT CLazer::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_fRange = 30.f;


	m_fFrame = 0.f;
	m_fLiveTime = 0.f;

	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);


	/*m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Green_Spin.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Green_Spin2.png", 2);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/laserbeam_R.png", 4);

	m_pTextureCom2->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Black_Spin.png", 1);
	m_pTextureCom2->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Black_Spin2.png", 2);
	m_pTextureCom2->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/laserbeam_modular.png", 4);*/

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	return S_OK;
}

Engine::_int CLazer::Update_GameObject(const _float& fTimeDelta)
{
	//if (m_bShooting) {

	//CollideMob = false;
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	if (m_bLateInit)
	{
		m_pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		m_pShotGun = dynamic_cast<CPaintShotGun*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER_GUN).front());
		m_bLateInit = false;
	}

	StartPosition();
	FirePosition(fTimeDelta);

	Mouse_Input();

	if (m_bFire)
	{
		__super::Update_GameObject(fTimeDelta);
	}





	return OBJ_NOEVENT;
}

void CLazer::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CLazer::Render_GameObject()
{
	if (m_bFire)
	{
		m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

		D3DCOLORVALUE Difuse = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Ambient = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Specular = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Emissive = { 1.f, 0.f, 0.f, 1.f };
		D3DMATERIAL9 Matirial = { Difuse, Ambient, Specular, Emissive };
		m_pGraphicDev->SetMaterial(&Matirial);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	}
}

void CLazer::OnCollisionEnter(CCollider* _pOther)
{
}

void CLazer::OnCollisionStay(CCollider* _pOther)
{
}

void CLazer::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CLazer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);
	//실제 충돌을 위한 CTransform 컴포넌트

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Lazer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	return S_OK;
}

void CLazer::Color_Select()
{
}

void CLazer::StartPosition()
{
 	m_fGunMoveDown = m_pShotGun->Get_GunMoveDown();
 	m_fGunMoveRight = m_pShotGun->Get_GunMoveRight();
 	m_vScale = m_pShotGun->Get_Scale();

 	//TODO 총이 플레이어와 상호작용하여 항상 일정한 위치 유지
 	_vec3 vPlayerPos, vPlayerLook, vPlayerUp;

 	m_pShotGun->Get_HostTransform()->Get_Info(INFO_UP, &vPlayerUp);
 	m_pShotGun->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);
 	m_pShotGun->Get_HostTransform()->Get_Info(INFO_LOOK, &vPlayerLook);
 	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

 	_vec3 vGunMove;

 	D3DXVec3Cross(&vGunMove, &vPlayerUp, &vPlayerLook);
 	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
 	D3DXVec3Normalize(&vGunMove, &vGunMove);

 	_vec3 vGunMoveRight = vGunMove / 20;
 	_vec3 vGunMoveDown = -vPlayerUp / 20;

 	_vec3 vPos = { vPlayerPos + vPlayerLook * 2.5f + vGunMoveRight * m_fGunMoveRight * 9.4f + vGunMoveDown * m_fGunMoveDown * 10.f };
//
 	m_pTransformCom->Set_Pos(vPos);
}

void CLazer::FirePosition(const _float& fTimeDelta)
{
	_vec3 vPlayerPos, vMyPos, vPlayerPos_Rel;
	m_pShotGun->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &m_vPos);
	vPlayerPos_Rel = vPlayerPos - m_vPos;
	D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
	_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z * 20);

	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI); // 자전

	m_pTransformCom->Move_Pos(&vPlayerPos_Rel, fTimeDelta, 1.f);
}

void CLazer::Mouse_Input()
{
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

}



CLazer* CLazer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLazer* pInstance = new CLazer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Lazer Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CLazer::Free()
{
	__super::Free();
}
