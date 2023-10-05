#include "../../Header/RigidBody.h"
#include "Export_Utility.h"


CRigidBody::CRigidBody()
{
}

CRigidBody::CRigidBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CRigidBody::CRigidBody(const CRigidBody& rhs)
	: CComponent(rhs)
{
}

CRigidBody::~CRigidBody()
{
	
}

HRESULT CRigidBody::Ready_RigidBody()
{
	ZeroMemory(&m_vNetPower, sizeof(_vec3));

	return S_OK;
}

_int CRigidBody::Update_RigidBody(const _float& fTimeDelta)
{
	m_fGravityAcc = m_fHeavy * -9.8f * 1.3f;
	if (m_IsKinematic)
	{
		//Get_Transform()->Translate(fTimeDelta * m_vNetPower);
		if (m_pTransformCom)
		{
			m_pTransformCom->Translate(fTimeDelta * m_vNetPower);
		}
	}	
	if (m_UseGravity)
	{
		m_vNetPower.y += fTimeDelta * m_fGravityAcc;
		// 임시 중력 해제
		/*if (m_pTransformCom->m_vInfo[INFO_POS].y >= m_pTransformCom->LocalScale().y + 2.1f)
			m_vNetPower.y += fTimeDelta * m_fGravityAcc;*/
		if (m_vNetPower.y < m_fGravityAcc)
			m_vNetPower.y = m_fGravityAcc;
	}
	/*else
	{
		Get_Transform()->Translate(fTimeDelta * m_vNetPower);

	}*/
	
	if (m_pTransformCom)
	{
		if (m_pTransformCom->m_vInfo[INFO_POS].y < 1.f)
		{
			m_pTransformCom->m_vInfo[INFO_POS].y = 1.f;
		}
	}
	

	//if (Get_Transform()->m_vInfo[INFO_POS].y < 1.f)
	//	Get_Transform()->m_vInfo[INFO_POS].y = 1.f;

	return 0;
}

CRigidBody* CRigidBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRigidBody* pInstance = new CRigidBody(pGraphicDev);

	if (FAILED(pInstance->Ready_RigidBody()))
	{
		Safe_Release(pInstance);
		MSG_BOX("RigidBody Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CRigidBody::Clone(void)
{
	return new CRigidBody(*this);
}

void CRigidBody::Free()
{
	CComponent::Free();
}
