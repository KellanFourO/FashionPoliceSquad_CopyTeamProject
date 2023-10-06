#include "stdafx.h"
#include "Rifle_Bullet1.h"

#include "Monster.h"

#include "Export_System.h"
#include "Export_Utility.h"

CRifle_Bullet1::CRifle_Bullet1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CRifle_Bullet1::CRifle_Bullet1(const CRifle_Bullet1& rhs)
	: CBullet(rhs)
{
}

CRifle_Bullet1::~CRifle_Bullet1()
{
	Free();
}

HRESULT CRifle_Bullet1::Ready_GameObject(_vec3 _StartPos, _int iColorIndex)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fBulletSpeed = 10.f;
	m_fLiveTime = 0.f;
	Shoot(_StartPos, iColorIndex);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);

	return S_OK;
}

Engine::_int CRifle_Bullet1::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bShooting) {
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		__super::Update_GameObject(fTimeDelta);

		Set_ObjectTag(OBJECTTAG::PLAYERBULLET);

		m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

		CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
		NULL_CHECK_RETURN(pPlayerTransCom, -1); // �÷��̾� ��������

		_vec3	vBulletScale = { 0.7f,0.7f,0.7f };
		m_pTransformCom->Set_Scale(vBulletScale); //ũ��

		m_vPos;
		_vec3 vPlayerPos, vMyPos, vPlayerPos_Rel;
		pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
		m_pTransformCom->Get_Info(INFO_POS, &m_vPos);
		vPlayerPos_Rel = vPlayerPos - m_vPos;
		D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
		_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);

		m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI); // ����

		m_pTransformCom->Set_Pos(m_vPos += (m_vDir * m_fBulletSpeed)); //�̵�

		m_fLiveTime += fTimeDelta;

		if (m_fLiveTime >= 2.f) {
			m_bShooting = false;
		}
	}
	return OBJ_NOEVENT;
}

void CRifle_Bullet1::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CRifle_Bullet1::Render_GameObject()
{
	if (m_bShooting) {
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pCollider->Render_Collider();
		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pTextureCom->Render_Textrue(0);
		//m_pBufferCom->Get_Frame(8, 1, m_iColorIndex);
		m_pBufferCom->Render_Buffer();
		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CRifle_Bullet1::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
	{
		m_bShooting = false;
		Set_ObjectTag(OBJECTTAG::DEAD_OBJ);

		if (dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_Info().iMobType ==
			MonsterType::BRIFBIG) {
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

void CRifle_Bullet1::OnCollisionStay(CCollider* _pOther)
{
}

void CRifle_Bullet1::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CRifle_Bullet1::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_R_Bullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	return S_OK;
}

void CRifle_Bullet1::Color_Select()
{
}

void CRifle_Bullet1::Shoot(_vec3 _StartPos, int iColorIndex)
{
	m_iColorIndex = iColorIndex;
	Color_Select();

	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
	NULL_CHECK(pPlayerTransCom); // �÷��̾� ��������

	pPlayerTransCom->Get_Info(INFO_LOOK, &m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pTransformCom->Set_Pos(_StartPos+ m_vDir);
	m_vPos = _StartPos;

	m_bShooting = true;
	m_fLiveTime = 0.f;
}

CRifle_Bullet1* CRifle_Bullet1::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _StartPos, _int iRandomIndex)
{
	CRifle_Bullet1* pInstance = new CRifle_Bullet1(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_StartPos, iRandomIndex)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Rifle_Bullet Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CRifle_Bullet1::Free()
{
	__super::Free();
}
