#include "stdafx.h"
#include "SYRay.h"

#include <algorithm>
#include "Export_System.h"
#include "Export_Utility.h"
#include "CollisionMgr.h"

CSYRay::CSYRay(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tRAY, sizeof(RAY));
}

CSYRay::CSYRay(const CSYRay& rhs)
	: Engine::CGameObject(rhs)
{
}

CSYRay::~CSYRay()
{
}




CSYRay* CSYRay::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pHost)
{
	CSYRay* pInstance = new CSYRay(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Ray Create Failed");
		return nullptr;
	}
	pInstance->m_pHost = _pHost;

	return pInstance;
}

HRESULT CSYRay::Add_Component()
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

void CSYRay::Free()
{
	__super::Free();
}



HRESULT CSYRay::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::RAY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(10.f, 10.f, 10.f));
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_POS], *m_pTransformCom->Get_Scale());

	return S_OK;
}

Engine::_int CSYRay::Update_GameObject(const _float& fTimeDelta)
{
	Renderer()->Add_RenderGroup(RENDER_UI,this);

	_int iExit = __super::Update_GameObject(fTimeDelta);

	if (m_bLateInit)
	{
		m_pPlayerTransform = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back()->Get_Transform();
		m_bLateInit = false;
	}

	m_tRAY = GetSYRay(m_pGraphicDev,m_pHost);

	m_pCollider->SetCenterPos(m_tRAY.vRayPos);

	auto& ObjList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::MONSTER);

	for (auto iter : ObjList)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Info().bDead)
			continue;

		if(CollisionManager()->CollisionRayToCube(m_pCollider,iter->Get_Collider(),iter->Get_Transform()->m_vInfo[INFO_POS]))
		{
			m_pTarget = iter;
			m_pHost->Set_bRay(true);
		}
		else
		{
			m_pTarget = nullptr;
			m_pHost->Set_bRay(false);
		}
	}


	return 0;
}

void CSYRay::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}
void CSYRay::Render_GameObject()
{
	m_pCollider->Render_Collider();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

}

RAY CSYRay::GetRay(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	_pGraphicDev->GetViewport(&ViewPort);


	// 뷰포트 변환 -> 투영 변환
	D3DXVECTOR3	vMouse;
	vMouse.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix		matView;

	_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	// matView = 뷰의 역 행렬

	RAY	tRay;


	tRay.vRayPos = { 0.f, 0.f, 0.f }; // -1 ~ 1
	tRay.vRayDir = vMouse - tRay.vRayPos;  // 뷰스페이스

	D3DXVec3TransformCoord(&tRay.vRayPos, &tRay.vRayPos, &matView);  // 0.0.0 * 뷰의 역행렬 = 월드
	D3DXVec3TransformNormal(&tRay.vRayDir, &tRay.vRayDir, &matView);

	return tRay;
}

RAY CSYRay::GetSYRay(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pGameObject)
{
	POINT ptPos;

	GetCursorPos(&ptPos);
	ScreenToClient(g_hWnd, &ptPos);

	_vec3 vScreenPos, vWorldPos;

	D3DVIEWPORT9 ViewPort;
	_pGraphicDev->GetViewport(&ViewPort);

	_matrix matProj, matView, matViewInver;
	_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matViewInver, nullptr, &matView);


	vScreenPos = { (float)ptPos.x, (float)ptPos.y, 0.0f };
	D3DXVec3Unproject(&vWorldPos, &vScreenPos, &ViewPort, &matProj, &matView, &matViewInver);

	RAY tRay;

	tRay.vRayPos = _pGameObject->Get_Transform()->m_vInfo[INFO_POS];
	tRay.vRayDir = vWorldPos - _pGameObject->Get_Transform()->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&tRay.vRayDir, &tRay.vRayDir);

	return tRay;

}


