#include "stdafx.h"
#include "PaintBall.h"

#include "Monster.h"

#include "Export_System.h"
#include "Export_Utility.h"

CPaintBall::CPaintBall(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CPaintBall::CPaintBall(const CPaintBall& rhs)
	: Engine::CGameObject(rhs)
{
}

CPaintBall::~CPaintBall()
{
	Free();
}

HRESULT CPaintBall::Ready_GameObject(_vec3 _StartPos, _int iColorIndex)
{
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fBulletSpeed = 10.f;
	m_fLiveTime = 0.f;
	Shoot(_StartPos, iColorIndex);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	return S_OK;
}

Engine::_int CPaintBall::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bShooting) {
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		__super::Update_GameObject(fTimeDelta);

		Set_ObjectTag(OBJECTTAG::PLAYERBULLET);

		m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

		CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		NULL_CHECK_RETURN(pPlayerTransCom, -1); // 플레이어 가져오기

		_vec3	vBulletScale = { 0.2f,0.2f,0.2f };
		m_pTransformCom->Set_Scale(vBulletScale); //크기

		m_vPos;
		_vec3 vPlayerPos, vMyPos, vPlayerPos_Rel;
		pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
		m_pTransformCom->Get_Info(INFO_POS, &m_vPos);
		vPlayerPos_Rel = vPlayerPos - m_vPos;
		D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
		_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);

		m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI); // 자전

		m_pTransformCom->Set_Pos(m_vPos += (m_vDir * m_fBulletSpeed)); //이동

		m_fLiveTime += fTimeDelta;

		if (m_fLiveTime >= 2.f) {
			m_bShooting = false;
			Set_ObjectTag(OBJECTTAG::DEAD_OBJ);

		}



	}
	
	
	return OBJ_NOEVENT;
}

void CPaintBall::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CPaintBall::Render_GameObject()
{
	if (m_bShooting) {


		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pCollider->Render_Collider();

		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Get_Frame(8, 1, m_iColorIndex);
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CPaintBall::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		m_bShooting = false;
		Set_ObjectTag(OBJECTTAG::DEAD_OBJ);
		if (dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_Info().iMobType ==
			MonsterType::BRIFSMALL) {
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked(5);
		}
		else {
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Attacked_Fail();
		}
	}
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
	{
		m_bShooting = false;
		Set_ObjectTag(OBJECTTAG::DEAD_OBJ);

	}
	int a = 1 + 1;
}

void CPaintBall::OnCollisionStay(CCollider* _pOther)
{
}

void CPaintBall::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CPaintBall::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PaintBall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	return S_OK;
}

void CPaintBall::Color_Select()
{
}

void CPaintBall::Shoot(_vec3 _StartPos, int iColorIndex)
{
	m_iColorIndex = iColorIndex;
	Color_Select();
	
	_vec3			vBulletDirMove;
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
	NULL_CHECK(pPlayerTransCom); // 플레이어 가져오기

	pPlayerTransCom->Get_Info(INFO_LOOK, &m_vDir);
	pPlayerTransCom->Get_Info(INFO_RIGHT,& vBulletDirMove);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_float m_fRange = 0.5f;
	if (rand() % 2 == 1) {
		m_fRange *= -1;
	}
	m_vDir.x += (rand() % 13) * 0.01f * m_fRange;
	if (rand() % 2 == 1) {
		m_fRange *= -1;
	}
	m_vDir.y += (rand() % 13) * 0.01f * m_fRange;
	if (rand() % 2 == 1) {
		m_fRange *= -1;
	}
	m_vDir.z += (rand() % 13) * 0.01f * m_fRange;
	D3DXVec3Normalize(&vBulletDirMove, &vBulletDirMove);
	m_vDir += _vec3(0.f, 0.06f, 0.f) - vBulletDirMove / 40;
	m_pTransformCom->Set_Pos(_StartPos);
	m_vPos = _StartPos;
	
	m_bShooting = true;
	m_fLiveTime = 0.f;
}

CPaintBall* CPaintBall::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 _StartPos, _int iColorIndex)
{
	CPaintBall* pInstance = new CPaintBall(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_StartPos, iColorIndex)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CPaintBall::Free()
{
	__super::Free();
}
