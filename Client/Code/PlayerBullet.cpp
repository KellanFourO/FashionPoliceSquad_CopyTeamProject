#include "stdafx.h"
#include "PlayerBullet.h"

CPlayerBullet::CPlayerBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_fLiveTime(0.f), m_fBulletMoveRight(0.f),m_fBulletMoveDown(0.f),m_fBulletSpeed(0.f)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet& rhs)
	:CGameObject(rhs)
{
}

CPlayerBullet::~CPlayerBullet()
{
}

HRESULT CPlayerBullet::Ready_GameObject(_vec3 _StartPos, _int iColorIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	


	return S_OK;
}

_int CPlayerBullet::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CPlayerBullet::LateUpdate_GameObject()
{
}

void CPlayerBullet::Render_GameObject()
{
}

void CPlayerBullet::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayerBullet::OnCollisionStay(CCollider* _pOther)
{
}

void CPlayerBullet::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CPlayerBullet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CPlayerBullet::Shoot(_vec3 _StartPos, int iColorIndex)
{
}

void CPlayerBullet::Free()
{
	__super::Free();
}
