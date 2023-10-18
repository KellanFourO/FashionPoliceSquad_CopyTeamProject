#include "stdafx.h"
#include "Ray.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Build_Cube.h"

CRay::CRay(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CRay::CRay(const CRay& rhs)
	:CGameObject(rhs)
{
}

CRay::~CRay()
{
}

HRESULT CRay::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::RAY_LASER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_POS], *m_pTransformCom->Get_Scale());
	m_pPlayerTransform = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	return S_OK;
}

_int CRay::Update_GameObject(const _float& fTimeDelta)
{
	//_vec3 test;
	//PerformRaycast(m_pPlayerTransform->m_vInfo[INFO_POS], m_pPlayerTransform->m_vInfo[INFO_LOOK], test ,40.f);
	m_pTransformCom->m_vInfo[INFO_POS] = m_pHost->m_pTransformCom->m_vInfo[INFO_LOOK] + _vec3(10.f,10.f,10.f);
	__super::Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CRay::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CRay::Render_GameObject()
{
	m_pCollider->Render_Collider();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->WorldMatrix());
}

void CRay::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_CUBE ||
		_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_OBJ ||
		_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER ||
		_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS)
	{

		_vec3 vHitPoint = _pOther->GetCenterPos();//�浹 ���� ���

		_float fDistanceToHit = D3DXVec3Length(&(vHitPoint - _pOther->GetCenterPos()));
		m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, fDistanceToHit));

	}
}

void CRay::OnCollisionStay(CCollider* _pOther)
{

}

void CRay::OnCollisionExit(CCollider* _pOther)
{

}
_bool CRay::PerformRaycast(_vec3 vStart, _vec3 vDir, _vec3& vHitPoint, _float fRayLength)
{

	_matrix matWorld, matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vOrigin, vDirection;
	_vec3 vRayDir;

	// ��ȯ ��Ʈ������ ����Ͽ� ����ĳ��Ʈ ���� ��ġ�� ������ ���� �����̽��� ��ȯ
	D3DXVec3TransformCoord(&vOrigin, &vStart, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vDir, &matWorld);

	// ������ ���� ���
	_vec3 vEnd = vOrigin + fRayLength * vRayDir;

	// ����-���� ��Ʈ������ ����Ͽ� ����ĳ��Ʈ ������ ��ũ�� �����̽��� ��ȯ
	_matrix matWorldViewProj = matWorld * matView * matProj;
	D3DXMatrixInverse(&matWorldViewProj, NULL, &matWorldViewProj);
	D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matWorldViewProj);
	D3DXVec3TransformCoord(&vEnd, &vEnd, &matWorldViewProj);

	vRayDir = vEnd - vOrigin; // ���� ���� ���
	D3DXVec3Normalize(&vRayDir, &vRayDir);


	// TODO: ����ĳ��Ʈ�� �浹 ���� �ڵ� ���� (�� �Ǵ� ��ü���� �浹 ����)
// 	_vec3 vEnd = vStart + vDir * fRayLength;
//
// 	// AABB�� �ּ� ���� �ִ� �� ��������
// 	_vec3 vMin = m_pCollider->Get_Min();
// 	_vec3 vMax = m_pCollider->Get_Max();
//
// 	// Ray�� ������ ����
// 	_vec3 vInvDir = 1.0f / vDir;
//
// 	// Ray�� t �� ���
// 	_vec3 tMin = (vMin - vStart) * vInvDir;
// 	_vec3 tMax = (vMax - vStart) * vInvDir;
//
// 	// tMin�� tMax ����
// 	if (tMin.x > tMax.x) std::swap(tMin.x, tMax.x);
// 	if (tMin.y > tMax.y) std::swap(tMin.y, tMax.y);
// 	if (tMin.z > tMax.z) std::swap(tMin.z, tMax.z);
//
// 	// tMax�� �ּ� ���� tMin�� �ִ� ������ ������ �������� ����
// 	if (tMax.x < tMin.y || tMax.x < tMin.z ||
// 		tMax.y < tMin.x || tMax.y < tMin.z ||
// 		tMax.z < tMin.x || tMax.z < tMin.y) {
// 		return false;
// 	}
//
// 	// Ray�� �������������� ���� ���� t �� ����
// 	_float tHit = (tMin.x > tMin.y) ? tMin.x : tMin.y;
// 	tHit = (tMin.z > tHit) ? tMin.z : tHit;
//
// 	// tHit�� 0���� �۰ų� tHit�� Ray ���̸� �ʰ��ϸ� �������� ����
// 	if (tHit < 0.0f || tHit > fRayLength) {
// 		return false;
// 	}
//
// 	// ���� ���� ���
// 	vHitPoint = vStart + vDir * tHit;

	return true;

	// ����ĳ��Ʈ �浹 ���� �ڵ� �ۼ�
	// ���� �浹�� �����Ǹ�, �浹 ������ vHitPoint�� �����ϰ� true ��ȯ
	// �浹�� �������� ������ false ��ȯ
}
CRay* CRay::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRay* pInstance = new CRay(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FootRay Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CRay::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CRay::Free()
{
	__super::Free();
}
