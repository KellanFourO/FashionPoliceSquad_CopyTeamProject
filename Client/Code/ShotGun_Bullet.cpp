#include "stdafx.h"
#include "ShotGun_Bullet.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "PaintBulletTrace.h"

CShotGunBullet::CShotGunBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CShotGunBullet::CShotGunBullet(const CShotGunBullet& rhs)
	: CBullet(rhs)
{
}

CShotGunBullet::~CShotGunBullet()
{
}

HRESULT CShotGunBullet::Ready_GameObject(_vec3 _StartPos, _int iColorIndex,COLORTAG pColorTag)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
	m_eBulletType = BULLETTYPE::SHOTGUN_BULLET;
	m_fSpeed = 300.f;
	m_fLiveTime = 0.f;
	m_fDmg = 10.f;
	m_iColorIndex = iColorIndex;
	m_pColorTag = COLORTAG(m_iColorIndex);

	m_pBufferCom->SetCount(8,1);
	//Set_ColorTag(COLORTAG::RED);
	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);




	_vec3	vBulletScale = { 0.3f,0.3f,0.3f };
	_vec3	vPos = { 9999.f,9999.f,9999.f };

	m_pTransformCom->Set_Pos(vPos);

	m_pTrace = CPaintBulletTrace::Create(m_pGraphicDev, m_pColorTag, m_pTransformCom->m_vInfo[INFO_POS]);
	m_pTrace->Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
	m_pTrace->Set_ShotDir(m_vShotDir);
	Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::PLAYERBULLET, m_pTrace);


	m_pTransformCom->Set_Scale(vBulletScale);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());
	m_pRigidBody->Set_Heavy(0.f);
	return S_OK;
}

Engine::_int CShotGunBullet::Update_GameObject(const _float& fTimeDelta)
{

		Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	if(m_bDead)
	{
		//if ((COLORTAG)m_iColorIndex == m_pColorTag)
		//{
		//	CPaintBulletTrace* pTrace = CPaintBulletTrace::Create(m_pGraphicDev, m_pColorTag,m_pTransformCom->m_vInfo[INFO_POS]);
		//	pTrace->Set_ObjectTag(OBJECTTAG::EFFECT);
		//	//pTrace->Set_ColorTag(m_pColorTag);
		//	pTrace->Set_ShotDir(m_vShotDir);
		//	Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT,pTrace);
			return OBJ_DEAD;
		//}
	}
	if(!m_pTrace->Get_Collision())
	m_pTrace->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);


	m_pRigidBody->Update_RigidBody(fTimeDelta);
		__super::Update_GameObject(fTimeDelta);


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

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer(m_iColorIndex,1);

}

void CShotGunBullet::OnCollisionEnter(CCollider* _pOther)
{
// 	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE)
// 	{
//
// 	}

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

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}


CShotGunBullet* CShotGunBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 _StartPos, _vec3 _vShotDir ,_int iColorIndex,COLORTAG pColorTag)
{
	CShotGunBullet* pInstance = new CShotGunBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_StartPos, iColorIndex,pColorTag)))
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
