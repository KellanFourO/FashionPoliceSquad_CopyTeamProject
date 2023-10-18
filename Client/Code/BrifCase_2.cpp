#include "stdafx.h"
#include "BrifCase_2.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBrifCase_2::CBrifCase_2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CBrifCase_2::CBrifCase_2(const CBrifCase_2& rhs)
	: CBullet(rhs)
{
}

CBrifCase_2::~CBrifCase_2()
{
}

HRESULT CBrifCase_2::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);

	m_vPos = m_pHostTransform->m_vInfo[INFO_POS];
	m_fDmg = 10.f;

	m_pTransformCom->Set_Host(this);
	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(_vec3{ 2.f,2.f,2.f });

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pBufferCom->SetCount(4,1);

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	m_fBulletSpeed = 0.6f;
	m_fAnimateTime = 0.f;
	m_fFrame = 0.f;
	m_fSpeed_Vertical = 0.1f;


	SoundMgr()->PlaySoundW(L"DullSuitBriefCaseThrow.wav",SOUND_MONSTER,0.5f);

	return S_OK;
}

Engine::_int CBrifCase_2::Update_GameObject(const _float& fTimeDelta)
{

		Engine::Add_RenderGroup(RENDER_ALPHA, this);
		//m_pRigidBody->Update_RigidBody(fTimeDelta);
		_int iExit = __super::Update_GameObject(fTimeDelta);

		m_pTransformCom->Get_Info(INFO_POS, &m_vPos);
		m_pTransformCom->Set_Pos(m_vPos += (m_vDir * m_fBulletSpeed)); //이동

		m_fAnimateTime += fTimeDelta;

		if (m_fAnimateTime >= 0.3f)
		{
			++m_fFrame;

			if (m_fFrame >= 4) {
				m_fFrame = 0;
			}
			m_fAnimateTime = 0.f;
		}//프레임 코드

	return iExit;
}

void CBrifCase_2::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CBrifCase_2::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer(m_fFrame,1);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CBrifCase_2::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);
}

void CBrifCase_2::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CBrifCase_2::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CBrifCase_2::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BrifCase2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

void CBrifCase_2::Shot(_vec3 _StartPos)
{
	_vec3 vPlayerPos;
	m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vDirection = vPlayerPos - _StartPos;

	D3DXVec3Normalize(&vDirection, &vDirection);

	_float fAngle = atan2f(vDirection.x, vDirection.z);

	m_pTransformCom->Set_Pos(_StartPos);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI); // 자전

	m_vPos = _StartPos;
	m_vDir = vDirection;
}



CBrifCase_2* CBrifCase_2::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pHostTransform, CTransform* pPlayerTransform)
{
	CBrifCase_2* pInstance = new CBrifCase_2(pGraphicDev);

	pInstance->m_pHostTransform = pHostTransform;
	pInstance->m_pPlayerTransformCom = pPlayerTransform;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BrifCase_2 Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CBrifCase_2::Free()
{
	SoundMgr()->StopSound(SOUND_MONSTER);
	__super::Free();
}
