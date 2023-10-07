#include "stdafx.h"
#include "Dyehard.h"
#include "PaintBall.h"
#include "MuzzleFlash.h"
#include "Player.h"

#include "ShotGunState.h"
#include "ShotGun_IDLE.h"
#include "ShotGun_WALK.h"
#include "ShotGun_JUMP.h"

#include "ShotGun_READY.h"
#include "ShotGun_Shot.h"
#include "ShotGun_RELOAD.h"

#include "ShotGun_Lazer.h"
#include "ShotGun_ENTER.h"



#include "Export_System.h"
#include "Export_Utility.h"

CDyehard::CDyehard(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGun(pGraphicDev)
{
}

CDyehard::CDyehard(const CDyehard& rhs)
	: CGun(rhs)
{
}

CDyehard::~CDyehard()
{
}

HRESULT CDyehard::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	ReadyState();

	m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(3.f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(3.f));

	m_bCharged		= false;
	m_bFire			= false;
	m_fBulletTime	= 0.f;
	m_fBulletCount	= 6.f;
	m_fMaxBullet	= 90.f;

	m_fGunMoveRight = 3;
	m_fGunMoveDown	= 1.f;

	GunState		= m_pStateArray[IDLE];
	//GunState->Initialize(this);

	GUN_ID			= GunID::DYEHARD;

	m_vGunScale		= { 1.3f,1.3f,1.3f }; // ÃÑ Å©±â

	m_bGun_Select	= true;
	m_bLazer = false;

	m_iBulletColor = 0;

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-dyehard-muzlflash2.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Guns/wpn-dyehard-charged2.png", 2);

	BulletList.clear();


	return S_OK;
}

Engine::_int CDyehard::Update_GameObject(const _float& fTimeDelta)
{
// 	if (m_bGun_Select)// ¼±ÅÃµÇ¾î¾ß ¾÷µ¥ÀÌÆ®/·»´õ µ¹¸²
// 		{
// 		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
//
// 		if (m_bLateInit)
// 		{
// 			m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
// 			GunState->Initialize(this);
// 			m_bLateInit = false;
// 		}
//
// 		dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(m_fBulletCount, m_fMaxBullet); // ÃÑ¾Ë °»½Å
//
// 		CGunState* State = GunState->Update(this, fTimeDelta);
// 		if (State != nullptr)
// 		{
// 			GunState = State;
// 			GunState->Initialize(this);
// 		} // »óÅÂ ÆÐÅÏ
//
//
//
// 		if (m_pPlayerTransformCom)
// 		{
// 			_vec3	vPlayerPos, vPlayerLook, vPlayerUp;
//
// 			m_pPlayerTransformCom->Get_Info(INFO_UP, &vPlayerUp);
// 			m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
// 			m_pPlayerTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);
// 			D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
//
// 			_vec3 vGunMove;
//
// 			D3DXVec3Cross(&vGunMove, &vPlayerUp, &vPlayerLook);
// 			D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
// 			D3DXVec3Normalize(&vGunMove, &vGunMove);
//
//
// 			_vec3 vGunMoveRight = vGunMove / 20;
// 			_vec3 vGunMoveDown = -vPlayerUp / 20;
//
// 			m_pTransformCom->Set_Scale(m_vGunScale);
// 			_vec3 vPos = { vPlayerPos + vPlayerLook * 2.5f + vGunMoveRight * m_fGunMoveRight * 9.4f + vGunMoveDown * m_fGunMoveDown * 10.f };
// 			//m_pTransformCom->Set_Pos(vPlayerPos + vPlayerLook*2.5f + vGunMoveRight * m_fGunMoveRight*9.4f + vGunMoveDown * m_fGunMoveDown*10.f);
// 			m_pTransformCom->Set_Pos(vPos);
//
// 			m_pTransformCom->Get_Info(INFO_POS, &vGunMove);
//
// 			m_fBulletTime += fTimeDelta;
//
// 			if (m_bFire == true)
// 			{m_fHP_Reduction
// 				m_fFireTime += fTimeDelta;
// 				if (m_fFireTime >= 0.035f)
// 				{
// 					m_bFire = false;
// 					m_fFireTime = 0.f;
// 				}
// 			}
// 		}
//
// 			Key_Input();
//
// 			if (false == m_bFix)
// 			{
// 				Mouse_Input();
// 			}
// 		}
//
//
// 	__super::Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CDyehard::LateUpdate_GameObject()
{
	if (BulletList.empty()) {
		for (int i = 0; i < 11; ++i) {
			CGameObject* Bullet = CPaintBall::Create(m_pGraphicDev,
				_vec3(0, 0, 0), m_iBulletColor); // »ö±ò¹Þ±â
			CManagement* pManagement;
			CLayer* pStageLayer = pManagement->GetInstance()->Get_Layer(LAYERTAG::GAMELOGIC);
			pStageLayer->Add_GameObject(OBJECTTAG::PLAYERBULLET, Bullet);
			BulletList.push_back(Bullet);
		}
	}
}

void CDyehard::Render_GameObject()
{
	if (m_bGun_Select) { // ¼±ÅÃµÇ¾î¾ß ¾÷µ¥ÀÌÆ®/·»´õ µ¹¸²
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		if (m_bFire) {
			m_pTextureCom->Render_Textrue(1);
		}
		else {
			m_pTextureCom->Render_Textrue(0);
		}// ÃÑÀÌ ¹ß»ç»óÅÂ¸é ÃÑ ÅØ½ºÃÄ°¡ ¹Ù²ñ
		m_pBufferCom->Render_Buffer();

		if (m_bCharged) {
			m_pTextureCom->Render_Textrue(2);
			m_pBufferCom->Render_Buffer(); // Â÷Áö
		}

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

HRESULT CDyehard::Add_Component()
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

void CDyehard::Mouse_Input()
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

void CDyehard::Key_Input()
{
	if (Engine::Get_DIKeyState(DIK_R) & 0x80 && m_fBulletCount != 6.f&&m_fTimeReload >= 0.4f) {
		m_bReload = true;
		m_fTimeReload = 0.f;
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

void CDyehard::ReadyState()
{
	m_pStateArray[IDLE]		= new CShotGun_IDLE;
	m_pStateArray[WALK]		= new CShotGun_WALK;
	m_pStateArray[JUMP]		= new CShotGun_JUMP;
	m_pStateArray[READY]	= new CShotGun_READY;
	m_pStateArray[SHOT]		= new CShotGun_SHOT;
	m_pStateArray[RELOAD]	= new CShotGun_RELOAD;
	m_pStateArray[LAZER]	= new CShotGun_Lazer;
	m_pStateArray[ENTER]	= new CShotGun_ENTER;
}


void CDyehard::Gun_Fire()
{
// 	_vec3 myvPos,m_vFirePos,vDir2;
// 	m_pTransformCom->Get_Info(INFO_POS, &myvPos);
// 	m_pPlayerTransformCom->Get_Info(INFO_POS, &m_vFirePos);
//
// 	vDir2 = dynamic_cast<CPlayer*>(m_pPlayer)->m_vMoveDir;
// 	m_vFirePos = (m_vFirePos + myvPos) / 2 + vDir2 +_vec3(0.f,-0.7f,0.f);
// 	//m_vFirePos = (m_vFirePos + myvPos) / 2;
// 	//m_vFirePos = (dynamic_cast<CPlayer*>(m_pPlayer)->Get_INFO()->vPos + m_vFirePos) / 2;
// 	if (m_bGun_Select) { // ¼±ÅÃµÇ¾î¾ß ¾÷µ¥ÀÌÆ®/·»´õ µ¹¸²
// 		if (Engine::Get_DIMouseState(DIM_LB) && m_fBulletTime >= 0.6f && m_bIsBullet)
// 		{
// 			for (auto iter : BulletList) {
// 				dynamic_cast<CPaintBall*>(iter)->
// 					Shoot(m_vFirePos, m_iBulletColor);
// 			}
//
// 			m_fBulletTime = 0.f;
// 			++m_iBulletColor;
// 			--m_fBulletCount;
// 			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bullet(m_fBulletCount, m_fMaxBullet);
// 			if (m_fBulletCount < 1) {
// 				m_bIsBullet = false;
// 			} // ÃÑ¾Ë ¹ß»ç
// 			if (m_iBulletColor >= 8) {
// 				m_iBulletColor = 0; // ÃÑ¾Ë »ö±ò ÃÊ±âÈ­
// 			}
// 			m_bFire = true;
// 			// ÃÑ¿°(¼¶±¤)
// 			GunState = m_pStateArray[SHOT]; // »óÅÂ¸¦ ÃÑ ½ð »óÅÂ·Î ¹Ù²Þ
// 			GunState->Initialize(this);   //Initialize ½ÇÇà
// 		}
// 		else if (Engine::Get_DIMouseState(DIM_RB))
// 		{
// 			m_bLazer = true;
// 			GunState = m_pStateArray[LAZER]; // »óÅÂ¸¦ ÃÑ ½ð »óÅÂ·Î ¹Ù²Þ
// 			GunState->Initialize(this);   //Initialize ½ÇÇà
// 		}
// 	}
}

CDyehard* CDyehard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDyehard* pInstance = new CDyehard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Dyehard Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CDyehard::Free()
{
	//Safe_Delete(GunState);

	for (auto iter : m_pStateArray)
		Safe_Delete(iter);

	BulletList.clear();
	__super::Free();
}
