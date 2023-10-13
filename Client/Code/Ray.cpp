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
	m_eObjectTag = OBJECTTAG::RAY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Get_Host();
	m_pTransformCom->Set_Scale(_vec3(0.1f, 0.2f, 0.1f));
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_POS], *m_pTransformCom->Get_Scale());
	return S_OK;
}

_int CRay::Update_GameObject(const _float& fTimeDelta)
{

	_int iExit = __super::Update_GameObject(fTimeDelta);
	//m_pHost->Set_Host(m_pPlayer);
	m_pTransformCom->m_vInfo[INFO_POS] = m_pHost->m_pTransformCom->m_vInfo[INFO_POS] + _vec3(0.f, -1.5f, 0.f);

	return iExit;
}

void CRay::LateUpdate_GameObject()
{
	//���߿� ���⿡ ���� ��ž�ҋ� ���⵵ �־�� �� 
	__super::LateUpdate_GameObject();
	//���� -> ����ٰ� �����ؼ� �Ϲȵ� UseGravity�� �߷��� ��ü ������Ʈ ���� �ֵ��� �̰� �����Ҽ� �ְ� ���� 
// 	if (!m_pColTarget)
// 	{
// 		if (!static_cast<CPlayer*>(m_pHost)->IsJump())
// 		{
// 			static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
// 			static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
// 		}
// 	}
}

void CRay::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->WorldMatrix());
}

void CRay::OnCollisionEnter(CCollider* _pOther)
{
	CPlayer& rPlayer = *Management()->Get_Scene()->Get_MainPlayer();

	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BUILD_CUBE == pOtherObj->Get_ObjectTag())
	{
		// ���� �浹�� ������ Ÿ�ٿ� ����
		m_pColTarget = pOtherObj;

	}
}

void CRay::OnCollisionStay(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
// 	if (OBJECTTAG::BUILD_CUBE == pOtherObj->Get_ObjectTag())
// 	{
// 		// ��� �浹 ���̶�� Host�� ���� ���� �� �ִ� ��. ���� ���� ����
// 		//static_cast<CPlayer*>(m_pHost)->Set_JumpState(false);
// 		static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(false);
// 	}
}

void CRay::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BUILD_CUBE == pOtherObj->Get_ObjectTag())
	{
		CPlayer& rPlayer = *Management()->Get_Scene()->Get_MainPlayer();

		// �浹 ������ ����� Ÿ�ٰ� ��ġ�Ѵٸ� �������� ���. ���� ���߿� �� �ִ� ����
		// ��ġ���� �ʴ´ٸ� �ٸ� �������� �̵��Ͽ� Enter�� ����ȣ��� ���̹Ƿ� nullptr�ʱ�ȭ �ʿ����
		/*if (pOtherObj == m_pColTarget)
		{
			m_pColTarget = nullptr;
			static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
			static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
		}*/

		m_pColTarget = nullptr;
		//static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
		//static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
	}
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