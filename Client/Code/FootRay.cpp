#include "stdafx.h"
#include "../Header/FootRay.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Build_Cube.h"

CFootRay::CFootRay(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CFootRay::CFootRay(const CFootRay& rhs)
	:CGameObject(rhs)
{
}

CFootRay::~CFootRay()
{
	Free();
}

HRESULT CFootRay::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::RAY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Get_Host();
	m_pTransformCom->Set_Scale(_vec3(0.000001f, 0.2f, 0.000001f));
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_POS], *m_pTransformCom->Get_Scale());
	return S_OK;
}

_int CFootRay::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = __super::Update_GameObject(fTimeDelta);
	//m_pHost->Set_Host(m_pPlayer);
	m_pTransformCom->m_vInfo[INFO_POS] = m_pHost->m_pTransformCom->m_vInfo[INFO_POS] + _vec3(0.f, -1.5f, 0.f);

	return iExit;
}

void CFootRay::LateUpdate_GameObject()
{
	//나중에 여기에 게임 스탑할떄 여기도 넣어야 함 
	__super::LateUpdate_GameObject();
	//점프 -> 여기다가 구현해서 하믄됨 UseGravity가 중력임 강체 컴포넌트 쓰는 애들은 이거 적용할수 있게 ㄱㄱ 
// 	if (!m_pColTarget)
// 	{
// 		if (!static_cast<CPlayer*>(m_pHost)->IsJump())
// 		{
// 			static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
// 			static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
// 		}
// 	}
}

void CFootRay::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->WorldMatrix());
}

void CFootRay::OnCollisionEnter(CCollider* _pOther)
{
	CPlayer& rPlayer = *Management()->Get_Scene()->Get_MainPlayer();

	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BUILD_CUBE == pOtherObj->Get_ObjectTag())
	{
		// 새로 충돌한 지형은 타겟에 저장
		m_pColTarget = pOtherObj;

	}
}

void CFootRay::OnCollisionStay(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BUILD_CUBE == pOtherObj->Get_ObjectTag())
	{
		// 계속 충돌 중이라면 Host가 블럭 위에 서 있는 것. 따라서 점프 ㄱㄴ
		//static_cast<CPlayer*>(m_pHost)->Set_JumpState(false);
		static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(false);
	}
}

void CFootRay::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BUILD_CUBE == pOtherObj->Get_ObjectTag())
	{
		CPlayer& rPlayer = *Management()->Get_Scene()->Get_MainPlayer();

		// 충돌 해제된 대상이 타겟과 일치한다면 블럭에서 벗어남. 따라서 공중에 떠 있는 상태
		// 일치하지 않는다면 다른 블럭위로 이동하여 Enter가 먼저호출된 것이므로 nullptr초기화 필요없음
		/*if (pOtherObj == m_pColTarget)
		{
			m_pColTarget = nullptr;
			static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
			static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
		}*/

		m_pColTarget = nullptr;
		//static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
		static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
	}
}

CFootRay* CFootRay::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFootRay* pInstance = new CFootRay(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FootRay Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CFootRay::Add_Component()
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

void CFootRay::Free()
{
	__super::Free();
}
