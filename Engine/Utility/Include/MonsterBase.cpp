#include "../../Header/MonsterBase.h"
#include "Export_Utility.h"


CMonsterBase::CMonsterBase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMonsterBase::CMonsterBase(const CMonsterBase& rhs)
	:CGameObject(rhs)
{
}

CMonsterBase::~CMonsterBase()
{
}

HRESULT CMonsterBase::Ready_GameObject()
{
	return S_OK;
}

_int CMonsterBase::Update_GameObject(const _float& fTimeDelta)
{
	_vec3 vPlayerPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransformCom->m_vInfo[INFO_POS];

// 	if (D3DXVec3Length(&(m_pTransformCom->m_vInfo[INFO_POS] - vPlayerPos)) > 110.f)
// 		return 0;

	if (m_pRigidBody)
		m_pRigidBody->Update_RigidBody(fTimeDelta);
	_int iExit = __super::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMonsterBase::OnCollisionEnter(CCollider* _pOther)
{
	_vec3	vOtherPos = _pOther->GetCenterPos();
	_float* fOtherAxis = _pOther->GetAxisLen();

	_vec3	vThisPos = m_pCollider->GetCenterPos();
	_float* fThisAxis = m_pCollider->GetAxisLen();


	// OBJECTTAG에 따른 예외 처리 가능성
	_float fWidth = fabs(vOtherPos.x - vThisPos.x);
	_float fHeight = fabs(vOtherPos.y - vThisPos.y);
	_float fDepth = fabs(vOtherPos.z - vThisPos.z);

	_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
	_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
	_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

	_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

	if (fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
		{
			if (m_pRigidBody)
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));

			m_pTransformCom->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));

// 			if (!m_bDieEffect)
// 				m_bDieEffect = true;
		}
		else
		{
			m_pTransformCom->Translate(_vec3(0.f, -fRadiusY, 0.f));
			//m_bDieEffect = false;
		}

	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransformCom->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransformCom->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
			m_pTransformCom->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransformCom->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
}

void CMonsterBase::OnCollisionStay(CCollider* _pOther)
{
	_vec3	vOtherPos = _pOther->GetCenterPos();
	_float* fOtherAxis = _pOther->GetAxisLen();

	_vec3	vThisPos = m_pCollider->GetCenterPos();
	_float* fThisAxis = m_pCollider->GetAxisLen();

	// OBJECTTAG에 따른 예외 처리 가능성
	_float fWidth = fabs(vOtherPos.x - vThisPos.x);
	_float fHeight = fabs(vOtherPos.y - vThisPos.y);
	_float fDepth = fabs(vOtherPos.z - vThisPos.z);

	_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
	_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
	_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

	_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

	if (fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
		{
			if (m_pRigidBody)
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));

			m_pTransformCom->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
			//m_bDieEffect = true;
		}
		else
		{
			m_pTransformCom->Translate(_vec3(0.f, -fRadiusY, 0.f));
			//m_bDieEffect = false;
		}
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransformCom->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransformCom->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
			m_pTransformCom->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransformCom->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
}

void CMonsterBase::OnCollisionExit(CCollider* _pOther)
{
}

void CMonsterBase::Free()
{
	__super::Free();
}
