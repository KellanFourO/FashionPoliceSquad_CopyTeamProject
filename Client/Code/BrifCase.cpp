#include "stdafx.h"
#include "BrifCase.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MBulletExplosion.h"
#include "Player.h"
#include "Monster.h"
#include "MBulletBombEffect.h"

CBrifCase::CBrifCase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CBrifCase::CBrifCase(const CBrifCase& rhs)
	: CBullet(rhs)
{
}

CBrifCase::~CBrifCase()
{
}

HRESULT CBrifCase::Ready_GameObject()
{
	m_bDead = false;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
	m_fDmg = 10.f;
	m_pBufferCom->SetCount(4, 1);

	m_pTransformCom->Set_Host(this);
	//m_pTransformCom->Set_Pos(m_pHostTransform->m_vInfo[INFO_POS] += m_vDir * 0.016);
	m_pTransformCom->Set_Pos(9999.f, 9999.f, 9999.f);
	m_pTransformCom->Set_Scale(_vec3{ 2.f,2.f,2.f });

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pRigidBody->Set_Host(this);
	m_pRigidBody->Set_Transform(m_pTransformCom);


	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());


	m_fBulletSpeed = 0.5f;

	m_fAnimateTime = 0.f;
	m_fFrame = 1.f;
	m_fSpeed_Vertical = 0.1f;
	SoundMgr()->PlaySoundW(L"BigDaddyBriefcaseFlyLoof.wav",SOUND_EFFECT3,0.5f);
	return S_OK;
}

Engine::_int CBrifCase::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		CMBulletExplosion* MBExplosion = CMBulletExplosion::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], 64);
		MBExplosion->Set_ObjectTag(OBJECTTAG::PARTICLE);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::PARTICLE, MBExplosion);
		MBExplosion->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		CMBulletBombEffect* MBulletEffect = CMBulletBombEffect::Create(m_pGraphicDev);
		MBulletEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBulletEffect);
		MBulletEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		SoundMgr()->StopSound(SOUND_EFFECT3);

		return OBJ_DEAD;
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pRigidBody->Update_RigidBody(fTimeDelta);

	_int iExit = __super::Update_GameObject(fTimeDelta);


	m_pTransformCom->Get_Info(INFO_POS, &m_vPos);

	m_pTransformCom->Set_Pos(m_vPos += (m_vDir * m_fBulletSpeed)); //이동

	m_fAnimateTime += fTimeDelta;

	if (m_fAnimateTime >= 0.25f)
	{
		++m_fFrame;

		if (m_fFrame >= 4) {
			m_fFrame = 1;
		}
		m_fAnimateTime = 0.f;
	}//프레임 코드


	return iExit;

}


void CBrifCase::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CBrifCase::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Textrue(0);

	m_pBufferCom->Render_Buffer(m_fFrame,1);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CBrifCase::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);
}
void CBrifCase::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CBrifCase::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CBrifCase::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BrifCase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

void CBrifCase::Shot(_vec3 _StartPos)
{

	m_pHostTransform->Get_Info(INFO_POS, &m_vPos);


	_vec3 vPlayerPos, vPlayerPos_Rel;
	m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_vDir = vPlayerPos - m_vPos;

	D3DXVec3Normalize(&m_vDir, &m_vDir);


	vPlayerPos_Rel = vPlayerPos - m_vPos;

	_float m_fSpeedLength = D3DXVec3Length(&vPlayerPos_Rel);
	m_fBulletSpeed = m_fSpeedLength / 100;
	if (m_fBulletSpeed >= 1) {
		m_fBulletSpeed = 1;
	}
	D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
	_float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);

	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI); // 자전

	m_pRigidBody->UseGravity(true,3.0f);

	m_pTransformCom->Set_Pos(_StartPos);
	m_vPos = _StartPos;
	m_bShooting = true;

	m_pRigidBody->Set_Force(_vec3(0.f, 25.f, 0.f));
}



CBrifCase* CBrifCase::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pHostTransform, CTransform* pPlayerTransform)
{

	CBrifCase* pInstance = new CBrifCase(pGraphicDev);

	pInstance->m_pHostTransform = pHostTransform;
	pInstance->m_pPlayerTransformCom = pPlayerTransform;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BrifCase Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CBrifCase::Free()
{
	__super::Free();
}
