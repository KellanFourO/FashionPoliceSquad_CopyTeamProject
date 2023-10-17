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

		_vec3 vHitPoint = _pOther->GetCenterPos();//충돌 지점 계산

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

	// 변환 매트릭스를 사용하여 레이캐스트 시작 위치와 방향을 월드 스페이스로 변환
	D3DXVec3TransformCoord(&vOrigin, &vStart, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vDir, &matWorld);

	// 레이의 끝점 계산
	_vec3 vEnd = vOrigin + fRayLength * vRayDir;

	// 역뷰-투영 매트릭스를 사용하여 레이캐스트 방향을 스크린 스페이스로 변환
	_matrix matWorldViewProj = matWorld * matView * matProj;
	D3DXMatrixInverse(&matWorldViewProj, NULL, &matWorldViewProj);
	D3DXVec3TransformCoord(&vOrigin, &vOrigin, &matWorldViewProj);
	D3DXVec3TransformCoord(&vEnd, &vEnd, &matWorldViewProj);

	vRayDir = vEnd - vOrigin; // 방향 벡터 계산
	D3DXVec3Normalize(&vRayDir, &vRayDir);


	// TODO: 레이캐스트와 충돌 검출 코드 구현 (맵 또는 객체와의 충돌 검출)
// 	_vec3 vEnd = vStart + vDir * fRayLength;
//
// 	// AABB의 최소 점과 최대 점 가져오기
// 	_vec3 vMin = m_pCollider->Get_Min();
// 	_vec3 vMax = m_pCollider->Get_Max();
//
// 	// Ray의 역방향 벡터
// 	_vec3 vInvDir = 1.0f / vDir;
//
// 	// Ray의 t 값 계산
// 	_vec3 tMin = (vMin - vStart) * vInvDir;
// 	_vec3 tMax = (vMax - vStart) * vInvDir;
//
// 	// tMin과 tMax 정렬
// 	if (tMin.x > tMax.x) std::swap(tMin.x, tMax.x);
// 	if (tMin.y > tMax.y) std::swap(tMin.y, tMax.y);
// 	if (tMin.z > tMax.z) std::swap(tMin.z, tMax.z);
//
// 	// tMax의 최소 값이 tMin의 최대 값보다 작으면 교차하지 않음
// 	if (tMax.x < tMin.y || tMax.x < tMin.z ||
// 		tMax.y < tMin.x || tMax.y < tMin.z ||
// 		tMax.z < tMin.x || tMax.z < tMin.y) {
// 		return false;
// 	}
//
// 	// Ray의 시작점에서부터 가장 작은 t 값 선택
// 	_float tHit = (tMin.x > tMin.y) ? tMin.x : tMin.y;
// 	tHit = (tMin.z > tHit) ? tMin.z : tHit;
//
// 	// tHit이 0보다 작거나 tHit이 Ray 길이를 초과하면 교차하지 않음
// 	if (tHit < 0.0f || tHit > fRayLength) {
// 		return false;
// 	}
//
// 	// 교차 지점 계산
// 	vHitPoint = vStart + vDir * tHit;

	return true;

	// 레이캐스트 충돌 검출 코드 작성
	// 만약 충돌이 감지되면, 충돌 지점을 vHitPoint에 저장하고 true 반환
	// 충돌이 감지되지 않으면 false 반환
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
