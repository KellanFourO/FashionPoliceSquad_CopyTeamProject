#include "stdafx.h"
#include "ShotGun_Bullet.h"

#include "Monster.h"

#include "Export_System.h"
#include "Export_Utility.h"

CShotGunBullet::CShotGunBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CShotGunBullet::CShotGunBullet(CShotGunBullet& rhs)
	: CBullet(rhs)
{
}

CShotGunBullet::~CShotGunBullet()
{
	Free();
}

HRESULT CShotGunBullet::Ready_GameObject(_vec3 _StartPos, _int iColorIndex)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::PLAYERBULLET);

	m_fSpeed = 100.f;
	m_fLiveTime = 0.f;
	m_fDmg = 10.f;


	m_pBufferCom->SetCount(8,1);

	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);



	_vec3	vBulletScale = { 0.3f,0.3f,0.3f };
	m_pTransformCom->Set_Scale(vBulletScale);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	return S_OK;
}

Engine::_int CShotGunBullet::Update_GameObject(const _float& fTimeDelta)
{

		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		__super::Update_GameObject(fTimeDelta);


			_vec3 vPlayerPos, vMyPos, vLook;

			m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
			m_pTransformCom->Get_Info(INFO_POS, &m_vPos);
			vLook = vPlayerPos - m_vPos;
			D3DXVec3Normalize(&vLook, &vLook);

			



			_float fAngle = atan2f(vLook.x, vLook.z);
			m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
			m_pTransformCom->Move_Pos(&m_vShotDir, fTimeDelta, m_fSpeed);

	return OBJ_NOEVENT;
}

void CShotGunBullet::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CShotGunBullet::Render_GameObject()
{
		m_pCollider->Render_Collider();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer(m_iColorIndex,1);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CShotGunBullet::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

}

void CShotGunBullet::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CShotGunBullet::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CShotGunBullet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
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

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}


CShotGunBullet* CShotGunBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 _StartPos, _int iColorIndex)
{
	CShotGunBullet* pInstance = new CShotGunBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_StartPos, iColorIndex)))
	{
		Safe_Release(pInstance);
		MSG_BOX("ShotGunBullet Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CShotGunBullet::Free()
{
	__super::Free();
}
