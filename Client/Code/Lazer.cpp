#include "stdafx.h"
#include "Lazer.h"

#include "Monster.h"
#include "Dyehard.h"

#include "Export_System.h"
#include "Export_Utility.h"

CLazer::CLazer(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CLazer::CLazer(const CLazer& rhs)
	: Engine::CGameObject(rhs)
{
}

CLazer::~CLazer()
{
	Free();
}

HRESULT CLazer::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fFrame = 0.f;
	m_fLiveTime = 0.f;

	m_pTransformCom4->Set_Scale(_vec3(10.f, 3.5f, 10.f));

	//m_pTransformCom->Rotation(ROT_X, -D3DXToRadian(50));
	//m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(50));

	//D3DXMatrixIdentity(&m_Mat_Axis);
	//D3DXMatrixRotationY(&m_Mat_Axis, -D3DXToRadian(50));
	//D3DXMatrixRotationZ(&m_Mat_Axis, -D3DXToRadian(50));

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom4);
	

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Green_Spin.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Green_Spin2.png", 2);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/laserbeam_R.png", 4);

	m_pTextureCom2->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Black_Spin.png", 1);
	m_pTextureCom2->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/Black_Spin2.png", 2);
	m_pTextureCom2->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/laserbeam_modular.png", 4);



	return S_OK;
}

Engine::_int CLazer::Update_GameObject(const _float& fTimeDelta)
{
	//if (m_bShooting) {

	CollideMob = false;
	if(dynamic_cast<CDyehard*>(m_pGun)->m_bLazer == true){
		Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
	}
	else {
		Set_ObjectTag(OBJECTTAG::DEAD_OBJ);
	}

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	m_fAnimateTime += fTimeDelta;
	m_fTestTime += fTimeDelta;
	m_fFrame += fTimeDelta * 10;
	
	if (dynamic_cast<CDyehard*>(m_pGun)->m_bLazer) {
		CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

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

		_vec3 vGunMoveRight = vGunMove / 20;
		_vec3 vGunMoveDown = -vPlayerUp / 20;

		float m_fGunMoveRight = 1.7;
		float m_fGunMoveDown = 1.0;


		m_pTransformCom->Set_Scale(_vec3(0.1f, 0.06f, 0.05f));
		m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook / 4 + vGunMoveRight * m_fGunMoveRight + vGunMoveDown * m_fGunMoveDown);

		//m_pTransformCom2->Set_Scale(_vec3(0.025f, 0.025f, 0.025f));
		//m_pTransformCom2->RotateAxis(vPlayerLook, D3DXToRadian(m_SpinAngle2 += 10));

		m_pTransformCom2->Set_Scale(_vec3(0.03f, 0.03f, 0.03f));
		//m_pTransformCom2->RotateAxis(vPlayerLook, D3DXToRadian(m_SpinAngle2 += 10));

		m_pTransformCom2->Set_Pos(vPlayerPos + vPlayerLook / 3);
		//m_pTransformCom2->Update_Component(fTimeDelta);

		m_pTransformCom3->Set_Scale(_vec3(0.025f, 0.025f, 0.025f));
		m_pTransformCom3->RotateAxis(vPlayerLook, D3DXToRadian(m_SpinAngle3 -= 15));
		m_pTransformCom3->Set_Pos(vPlayerPos + vPlayerLook / 5 + vGunMoveRight * m_fGunMoveRight * 0.75 + vGunMoveDown * m_fGunMoveDown * 0.75);

		//D3DXVec3TransformNormal(&vPlayerLook,&vPlayerLook,&m_Mat_Axis);
		m_pTransformCom->RotateAxis(vPlayerLook, D3DXToRadian(150));

		m_pTransformCom4->Set_Pos(vPlayerPos + vPlayerLook * 25);

	}
	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_matrix		matRot;
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
		m_pTransformCom2->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
		m_pTransformCom3->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
		m_pTransformCom4->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));


	}
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_matrix		matRot;

		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
		m_pTransformCom2->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
		m_pTransformCom3->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
		m_pTransformCom4->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));

	}

	if (Engine::Get_DIKeyState(DIK_B) && m_fTestTime >= 0.2f) {
		m_fTestTime = 0;
		CollideMob = !CollideMob;
	}

	m_pCollider->InitOBB(m_pTransformCom4->m_vInfo[INFO_POS], &m_pTransformCom4->m_vInfo[INFO_RIGHT], *m_pTransformCom4->Get_Scale());

	__super::Update_GameObject(fTimeDelta);


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
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

	CTexture* pTextureCom_Current = nullptr;
	if (CollideMob == true) {
		pTextureCom_Current=m_pTextureCom;
	}
	else {
		pTextureCom_Current = m_pTextureCom2;
	}
		D3DCOLORVALUE Difuse = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Ambient = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Specular = { 1.f, 0.f, 0.f, 1.f };
		D3DCOLORVALUE Emissive = { 1.f, 0.f, 0.f, 1.f };
		D3DMATERIAL9 Matirial = { Difuse, Ambient, Specular, Emissive };
		m_pGraphicDev->SetMaterial(&Matirial);
	if (dynamic_cast<CDyehard*>(m_pGun)->m_bLazer) {
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom4->Get_WorldMatrix());
		m_pCollider->Render_Collider();
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

			pTextureCom_Current->Render_Textrue(4);
			m_pBufferCom->Get_WaterFrame(m_fFrame);
			//m_pBufferCom->fAdditionalTexture = 2.0f;

			m_pBufferCom->Render_Buffer();
		}//main Lazer

		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom2->Get_WorldMatrix());
			m_pBufferCom->Get_WaterFrame(0);
			pTextureCom_Current->Render_Textrue(2);
			m_pBufferCom->Render_Buffer();

		}//Spin Lazer(에임쪽)

		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom3->Get_WorldMatrix());
			//m_pBufferCom->Get_WaterFrame(0);
			pTextureCom_Current->Render_Textrue(1);
			m_pBufferCom->Render_Buffer();
		}//Spin Lazer(총 쪽)
	}


	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
}

void CLazer::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
	{
		CollideMob = false;
	}
	int a = 1 + 1;
}

void CLazer::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
	{
		CollideMob = false;
	}
	
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		if (dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_Info().iMobType ==
			MonsterType::KCIKBOARD) {
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(1);
			CollideMob = true;
		}
		else {
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked_Fail();
		}
	}
}

void CLazer::OnCollisionExit(CCollider* _pOther)
{
	CollideMob = false;

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

	pComponent = m_pTransformCom2 = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM_2, pComponent);

	pComponent = m_pTransformCom3 = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM_3, pComponent); // 컴포넌트에 넣어야해서 대충적음
	
	pComponent = m_pTransformCom4 = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM_4, pComponent); // 컴포넌트에 넣어야해서 대충적음
	//실제 충돌을 위한 CTransform 컴포넌트

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Lazer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Lazer"));
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

void CLazer::Shoot(_vec3 _StartPos, int iColorIndex)
{

	m_pTransformCom->Set_Pos(_StartPos);
	m_vPos = _StartPos;

	m_bShooting = true;
	m_fLiveTime = 0.f;
}

CLazer* CLazer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLazer* pInstance = new CLazer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CLazer::Free()
{
	__super::Free();
}
