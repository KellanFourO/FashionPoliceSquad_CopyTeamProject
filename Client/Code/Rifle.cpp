#include "stdafx.h"
#include "Rifle.h"
#include "Rifle_Bullet.h"
#include "MuzzleFlash.h"
#include "Player.h"

#include "RifleState.h"
#include "Rifle_IDLE.h"
#include "Rifle_SHOT.h"


#include "Export_System.h"
#include "Export_Utility.h"

CRifle::CRifle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGun(pGraphicDev)
{
}

CRifle::CRifle(const CRifle& rhs)
	: CGun(rhs)
{
}

CRifle::~CRifle()
{
}

HRESULT CRifle::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(3.f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(3.f));

	m_bCharged = false;
	m_bFire = false;
	m_fBulletTime = 0.f;
	m_fBulletCount = 50.f;
	m_fMaxBullet = 500.f;

	m_fGunMoveRight = 3;
	m_fGunMoveDown = 1.f;

	iBulletNum = 0;

	GunState = new CRifle_IDLE;

	m_vGunScale = { 1.3f,1.3f,1.3f }; // 총 크기

	m_bGun_Select = false;

	GUN_ID = GunID::RIFLE;

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-tailormade-body-flash2.png", 1);

	return S_OK;
}

Engine::_int CRifle::Update_GameObject(const _float& fTimeDelta)
{
// 	if (m_bGun_Select) { // 선택되어야 업데이트/렌더 돌림
// 		Engine::Add_RenderGroup(RENDER_ALPHA, this);
// 		dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(m_fBulletCount, m_fMaxBullet);
//
// 		CGunState* State = GunState->Update(this, fTimeDelta);
// 		if (State != nullptr) {
// 			GunState->Release(this);
// 			Safe_Delete(GunState);
// 			GunState = State;
// 			GunState->Initialize(this);
// 		} // 상태 패턴
//
//
//
//
// 		CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
//
// 		NULL_CHECK_RETURN(pPlayerTransCom, -1);
//
// 		_vec3	vPlayerPos, vPlayerLook, vPlayerUp;
//
// 		pPlayerTransCom->Get_Info(INFO_UP, &vPlayerUp);
// 		pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
// 		pPlayerTransCom->Get_Info(INFO_LOOK, &vPlayerLook);
// 		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
//
// 		_vec3 vRifleMove;
//
// 		D3DXVec3Cross(&vRifleMove, &vPlayerUp, &vPlayerLook);
// 		D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
// 		D3DXVec3Normalize(&vRifleMove, &vRifleMove);
//
// 		_vec3 vRifleMoveRight = vRifleMove / 20;
// 		_vec3 vRifleMoveDown = -vPlayerUp / 20;
//
// 		m_pTransformCom->Set_Scale(m_vGunScale);
// 		m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook * 2.5f + vRifleMoveRight * m_fGunMoveRight * 8.0f + vRifleMoveDown * m_fGunMoveDown * 14.5f);
//
// 		m_fBulletTime += fTimeDelta;
//
// 		if (m_bFire == true) {
// 			m_fFireTime += fTimeDelta;
// 			if (m_fFireTime >= 0.1f) {
// 				m_bFire = false;
// 				m_fFireTime = 0.f;
// 			}
// 		}
//
// 	}
// 	Key_Input();
// 	if (false == m_bFix)
// 	{
// 		Mouse_Input();
// 	}
// 	__super::Update_GameObject(fTimeDelta);
 	return OBJ_NOEVENT;
}

void CRifle::LateUpdate_GameObject()
{

}

void CRifle::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Textrue(0);

	if (m_bFire) {
		m_pTextureCom->Render_Textrue(1);

	}
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CRifle::Add_Component()
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

void CRifle::Mouse_Input()
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

void CRifle::Key_Input()
{
	if (m_bGun_Select) { // 선택되어야 업데이트/렌더 돌림
		if (Engine::Get_DIKeyState(DIK_R) & 0x80 && m_fBulletCount != 50.f&& m_fTimeReload >= 0.4f) {
			m_bReload = true;
			m_fTimeReload = 0.f;
		}
	}
	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;


	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;
}

void CRifle::Gun_Fire()
{
// 	_vec3 myvPos, m_vFirePos, vDir2;
// 	m_pTransformCom->Get_Info(INFO_POS, &myvPos);
// 	dynamic_cast<CPlayer*>(m_pPlayer)->Get_Transform()->Get_Info(INFO_POS, &m_vFirePos);
// 	vDir2 = dynamic_cast<CPlayer*>(m_pPlayer)->m_vMoveDir;
// 	m_vFirePos = (m_vFirePos + myvPos) / 2 + vDir2;
//
// 	if (m_bGun_Select) { // 선택되어야 업데이트/렌더 돌림
// 		if (Engine::Get_DIMouseState(DIM_LB) && m_fBulletTime >= 0.15f && m_bIsBullet)
// 		{
// 			if (BulletList.empty()) {
// 				for (int i = 0; i < 5; ++i) {
// 					CGameObject* Bullet = CRifle_Bullet::Create(m_pGraphicDev,
// 						m_vFirePos, 0); // 색깔받기
// 					CManagement* pManagement;
// 					CLayer* pStageLayer = pManagement->GetInstance()->Get_Layer(LAYERTAG::GAMELOGIC);
// 					pStageLayer->Add_GameObject(OBJECTTAG::PLAYERBULLET, Bullet);
// 					BulletList.push_back(Bullet);
// 				}
// 				auto iter = BulletList.begin();
// 				for (int i = 0; i < iBulletNum; i++) {
// 					++iter;
// 				}
// 				dynamic_cast<CRifle_Bullet*>(*iter)->
// 					Shoot(m_vFirePos, 0);
// 			}
// 			else {
// 				auto iter = BulletList.begin();
// 				for (int i = 0; i < iBulletNum; i++) {
// 					++iter;
// 				}
// 				dynamic_cast<CRifle_Bullet*>(*iter)->
// 					Shoot(m_vFirePos, 0);
// 			}
// 			//총알 발사 코드 넣기
// 			m_fBulletTime = 0.f;
// 			--m_fBulletCount;
// 			++iBulletNum;
// 			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(m_fBulletCount, m_fMaxBullet);
// 			if (m_fBulletCount < 1) {
// 				m_bIsBullet = false;
// 			} // 총알 발사
// 			if (iBulletNum >= 5) {
// 				iBulletNum = 0;
// 			}
// 			m_bFire = true;
// 			// 총염 (섬광)
// 			GunState->Release(this);
// 			Safe_Delete(GunState);
// 			GunState = new CRifle_SHOT; // 상태를 총 쏜 상태로 바꿈
// 			GunState->Initialize(this);   //Initialize 실행
// 		}
// 	}
}


CRifle* CRifle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRifle* pInstance = new CRifle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CRifle::Free()
{
	Safe_Delete(GunState);
	__super::Free();
}
