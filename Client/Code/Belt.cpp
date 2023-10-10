#include "stdafx.h"
#include "Belt.h"
#include "Belt_IDLE.h"
#include "PaintBall.h"
#include "MuzzleFlash.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBelt::CBelt(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}


CBelt::CBelt(const CBelt& rhs)
	: Engine::CGameObject(rhs)
{
}

CBelt::~CBelt()
{
}

HRESULT CBelt::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fBeltMoveRight = 3;
	m_fBeltMoveDown = 5.f;
	m_vBeltScale = { 0.3f,0.3f,0.3f };

	BeltState = new CBelt_IDLE;
	BeltState->Initialize(this);
	m_pTransformCom->Set_Rotate(ROT_Z,0);

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Belt-charged.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Belt2.png", 2);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Belt2-swinging.png", 3);


	return S_OK;
}

Engine::_int CBelt::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	if (m_bLateInit)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER).back());
		m_pPlayerTransform = m_pPlayer->Get_Transform();
		m_bLateInit = false;
	}
	Rope();

	CBeltState* State = BeltState->Update(this, fTimeDelta);
	if (State != nullptr) {
		BeltState->Release(this);
		Safe_Delete(BeltState);
		BeltState = State;
		BeltState->Initialize(this);
	} // 상태 패턴

	_vec3	vPlayerPos, vPlayerUp;

	m_pPlayerTransform->Get_Info(INFO_UP, &vPlayerUp);
	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pPlayerTransform->Get_Info(INFO_LOOK, &m_vPlayerLook);
	D3DXVec3Normalize(&m_vPlayerLook, &m_vPlayerLook);


	_vec3 vGunMove;

	D3DXVec3Cross(&vGunMove, &vPlayerUp, &m_vPlayerLook);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	D3DXVec3Normalize(&vGunMove, &vGunMove);




	_vec3 vGunMoveRight = vGunMove / 20;
	_vec3 vGunMoveDown = -vPlayerUp / 20;


	m_pTransformCom->Set_Scale(m_vBeltScale);
	m_pTransformCom->Set_Pos(vPlayerPos + m_vPlayerLook /3 - vGunMoveRight * m_fBeltMoveRight*1.3f + vGunMoveDown * m_fBeltMoveDown * 1.7f);







	Key_Input();
	Mouse_Input(fTimeDelta);


	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CBelt::LateUpdate_GameObject()
{
}

void CBelt::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	if (m_bHit == true) {
		m_pTextureCom->Render_Textrue(2);
	}
	else {
		m_pTextureCom->Render_Textrue(0);
	}
	m_pBufferCom->Render_Buffer();

	if (m_bCharged) {
		m_pTextureCom->Render_Textrue(1);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CBelt::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Belt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CBelt::Mouse_Input(const _float& fTimeDelta)
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

void CBelt::Key_Input()
{


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

void CBelt::Rope()
{

}


CBelt* CBelt::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBelt* pInstance = new CBelt(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CBelt::Free()
{
	Safe_Delete(BeltState);
	__super::Free();
}
