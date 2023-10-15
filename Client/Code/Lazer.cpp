#include "stdafx.h"
#include "Lazer.h"
#include "PaintShotGun.h"
#include "Export_System.h"
#include "Export_Utility.h"

CLazer::CLazer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CLazer::CLazer(CLazer& rhs)
	: CBullet(rhs)
{
}

CLazer::~CLazer()
{
}

HRESULT CLazer::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::PLAYER_LAZER);

	m_fRange = 100.f;
	m_eBulletType = BULLETTYPE::SHOTGUN_RAZER;

	m_pBufferCom->SetCount(4,1);
	m_fFrame = 0.f;
	m_fLiveTime = 0.f;
	m_fTestTime = 0.06f;
	m_fDmg = 1.f;
	m_pTransformCom->Set_Host(this);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	m_pTransformCom->Set_Scale(_vec3(70.f, 0.1f, 0.05f));
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	//m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, 0.05f));
	return S_OK;
}

Engine::_int CLazer::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	if (m_bLateInit)
	{
		m_pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		m_pShotGun = dynamic_cast<CPaintShotGun*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER_GUN).front());
		m_bLateInit = false;
	}

	m_fAnimateTime += fTimeDelta;

	if (m_fAnimateTime > 0.04f)
	{

		++m_fFrame;
		m_fTestTime += 0.004f;
		//m_pTransformCom->Set_Scale(_vec3(40.f, m_fTestTime, 1.f));
		m_fAnimateTime = 0;
		if (m_fFrame > 10)
		{
			m_fFrame = 0;
			m_fTestTime = 0.1f;
			//m_pTransformCom->Set_Scale(_vec3(40.f, m_fTestTime, 1.f));
		}
	}
	_vec3 vStart = m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vDir = m_pTransformCom->m_vInfo[INFO_LOOK];
	_vec3 vHitPoint;


	StartPosition();
	FirePosition(fTimeDelta);

	Mouse_Input();



	if (m_bFire)
	{
		__super::Update_GameObject(fTimeDelta);
		m_pCollider->SetCenterPos(vStart);
	}
	else
	{
		_vec3 vGabagePos = { 9999.f, 9999.f, 9999.f};
		m_pCollider->SetCenterPos(vGabagePos);
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
		m_pCollider->Render_Collider();
		m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

		D3DCOLORVALUE Difuse = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Ambient = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Specular = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Emissive = { 1.f, 0.f, 0.f, 1.f };
		D3DMATERIAL9 Matirial = { Difuse, Ambient, Specular, Emissive };
		m_pGraphicDev->SetMaterial(&Matirial);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer(m_fFrame,1);

		m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	}
}

void CLazer::OnCollisionEnter(CCollider* _pOther)
{
	if ( _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER	)
	{
		_vec3 vHitPoint = _pOther->GetCenterPos();//충돌 지점 계산

		_float fDistanceToHit = D3DXVec3Length(&(vHitPoint - m_pTransformCom->m_vInfo[INFO_POS]));

		if(_pOther->Get_Host()->Get_HitType() == BULLETTYPE::SHOTGUN_RAZER)
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(m_fDmg);

	}

}

void CLazer::OnCollisionStay(CCollider* _pOther)
{
	if ( _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		_vec3 vHitPoint = _pOther->GetCenterPos();//충돌 지점 계산

		_float fDistanceToHit = D3DXVec3Length(&(vHitPoint - m_pTransformCom->m_vInfo[INFO_POS]));

		if (_pOther->Get_Host()->Get_HitType() == BULLETTYPE::SHOTGUN_RAZER)
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(m_fDmg);

	}
}

void CLazer::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CLazer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
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

	float m_fGunMoveRight = 1.5;
	float m_fGunMoveDown = 1.0f;


	_vec3 vStartPos = vPlayerPos + vPlayerLook / 4 + vGunMoveRight * 1.5 + vGunMoveDown * 1.0f;


	m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook/4 + vGunMoveRight * m_fGunMoveRight + vGunMoveDown * m_fGunMoveDown);
	m_pTransformCom->RotateAxis(vPlayerUp, D3DXToRadian(90));

}

void CLazer::FirePosition(const _float& fTimeDelta)
{
}

void CLazer::SearchRangeTarget()
{
// 	auto& ObjectList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::MONSTER);
//
// 	for (auto iter : ObjectList)
// 	{
// 		_vec3 vMonsterPos;
// 		iter->Get_Transform()->Get_Info(INFO_POS, &vMonsterPos);
//
// 		_float fDistance = D3DXVec3Length(&(vMonsterPos - m_pTransformCom->m_vInfo[INFO_POS]));
//
// 		if (fDistance < m_fRange)
// 		{
//
// 		}
//
//
// 	}


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
