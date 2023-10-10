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
}

HRESULT CRifle_Bullet1::Ready_GameObject(_vec3 _StartPos, _int iColorIndex)
{

	Set_ObjectTag(OBJECTTAG::PLAYERBULLET);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eBulletType = BULLETTYPE::ASSERTRIFLE_BULLET;

	m_fSpeed = 110.f;
	m_fDmg = 10.f;



	_vec3 vScale = { 0.7f, 0.7f, 0.7f};

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Host(this);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());


	return S_OK;
}

Engine::_int CRifle_Bullet1::Update_GameObject(const _float& fTimeDelta)
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

void CRifle_Bullet1::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CRifle_Bullet1::Render_GameObject()
{

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Render_Textrue(0);
		m_pBufferCom->Render_Buffer();
}

void CRifle_Bullet1::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);
}

void CRifle_Bullet1::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CRifle_Bullet1::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RifleBullet"));
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


CRifle_Bullet1* CRifle_Bullet1::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _StartPos, _int iRandomIndex)
{
	CRifle_Bullet1* pInstance = new CRifle_Bullet1(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_StartPos, iRandomIndex)))
	{
		Safe_Release(pInstance);
		MSG_BOX("RifleBullet Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CRifle_Bullet1::Free()
{
	__super::Free();
}
