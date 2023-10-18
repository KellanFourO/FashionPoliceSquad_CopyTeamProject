#include "stdafx.h"
#include "UI_RecognitionRange.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"
#include "Monster.h"
#include "FontMgr.h"

#include <DirectXMath.h>

CRecognitionRange::CRecognitionRange(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CRecognitionRange::CRecognitionRange(const CRecognitionRange& rhs)
	: CGameObject(rhs)
{
}

CRecognitionRange::~CRecognitionRange()
{
}

HRESULT Engine::CRecognitionRange::Ready_GameObject()
{
 	D3DXMatrixIdentity(&m_matView);
 	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f,2.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vTargetPos = {0.f,0.f,0.f};

	TextureSet();

	//if (!m_pFrustum)
	//{
	//	m_pFrustum = new CFrustum;
	//	m_pFrustum->MakeFrustum(Management()->Get_Camera()->Get_ViewProj());
	//}


	m_pPlayerTransformCom = Management()->Get_Player()->Get_Transform();

	switch (m_eUIType)
	{
	case Engine::UI_TYPE::DESTINATION:
		m_pTransformCom->Set_Scale(_vec3(10.f, 10.f,1.f));
		break;
	case Engine::UI_TYPE::TARGETPOS:
		m_pTransformCom->Set_Scale(_vec3(8.f, 8.f, 1.f));
		m_fRenderDistance = 200.f;
		break;
	case Engine::UI_TYPE::RECOG:
		m_pTransformCom->Set_Scale(_vec3(8.f, 8.f, 1.f));
		break;
	}
	return S_OK;
}

Engine::_int Engine::CRecognitionRange::Update_GameObject(const _float& fTimeDelta)
{
	//	TODO - 몬스터의 상태값을 가져온다고 가정한상황 ( 발견, 미발견, 스턴 )
	if(m_eUIType != UI_TYPE::DESTINATION)
	DistanceRender();

	//AngleRender();


	if (m_IsDead)
	{
		return OBJ_DEAD;
	}

	if(m_bDistanceRender && m_bAngleRender)
	Engine::Add_RenderGroup(RENDER_UI, this);
	else if (m_eUIType == UI_TYPE::DESTINATION)
		Engine::Add_RenderGroup(RENDER_UI, this);

	switch (m_eUIType)
	{
	case Engine::UI_TYPE::DESTINATION:
	{
		if (m_bTargetPos)
		{
			DestinationUI(fTimeDelta);
		}
		else
			MSG_BOX("너 목적지UI 타겟셋팅 안했어.");
		break;
	}

	case Engine::UI_TYPE::TARGETPOS:
	{
		if(m_bDistanceRender && m_bAngleRender)
		TargetUI(fTimeDelta);
		break;
	}

	case Engine::UI_TYPE::RECOG:
	{
		if (m_bDistanceRender && m_bAngleRender)
		RecogUI(fTimeDelta);

		if (m_bRecogDetect)
		{
			m_fRecogAge += fTimeDelta;

			if(m_fRecogAge > m_fRecogLifeTime && static_cast<CMonster*>(m_pTarget)->Get_Info().bDead)
			return OBJ_DEAD;
		}
		break;
	}
	}


	_int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void Engine::CRecognitionRange::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CRecognitionRange::Render_GameObject()
{
	switch (m_eUIType)
	{
		case Engine::UI_TYPE::DESTINATION:
		{
			if (m_bTargetPos)
			{
				DestinationRender();
			}
			else
				MSG_BOX("너 목적지UI 타겟셋팅 안했어.");

			break;
		}

		case Engine::UI_TYPE::TARGETPOS:
		{
			if (m_bDistanceRender && m_bAngleRender)
			TargetRender();
			break;
		}

		case Engine::UI_TYPE::RECOG:
		{
			if (m_bDistanceRender && m_bAngleRender)
			RecogRender();
			break;
		}
	}
}

HRESULT Engine::CRecognitionRange::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RecogUITexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}


void CRecognitionRange::DestinationUI(const _float& fTimeDelta)
{
	_matrix matWorld, matView, matProj, matViewProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);



	_vec3 vWindowPos = m_vTargetPos;

	vWindowPos.y += 5.f;

	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matView); // 기존 뷰,투영 행렬로 곱
	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matProj);	// 기존 뷰,투영 행렬로 곱

	vWindowPos.x *= WINCX * 0.5f;
	vWindowPos.y *= WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vWindowPos);
	m_pTransformCom->Set_Scale(_vec3(10.f, 10.f, 1.f));


	_vec3 vPlayerPos;
	m_pPlayerTransformCom->Get_Info(INFO_POS,&vPlayerPos);

	_vec3 vPlayerToDesti = m_vTargetPos - vPlayerPos;

	m_fDestinationDistance = D3DXVec3Length(&vPlayerToDesti);
	//todo 시작 지점으로부터 287

 	D3DVIEWPORT9 ViewPort;
 	m_pGraphicDev->GetViewport(&ViewPort);

 	_vec3 vScreenPos;
 	D3DXVec3Project(&vScreenPos,&m_vTargetPos,&ViewPort,&matProj,&matView,m_pTransformCom->Get_WorldMatrix());

 	m_vDestinationTextPos.x = vScreenPos.x;
 	m_vDestinationTextPos.y = -vScreenPos.y;
 	m_vDestinationTextPos.y = m_vDestinationTextPos.y + 5.f;
	//_D3DVECTOR = {x=440.264557 y=32.0448914 z=0.999643147 }
	_int i = 0;
}

void CRecognitionRange::RecogUI(const _float& fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(m_pTarget);

	if (pMonster->Get_Info().bDead)
	{
		m_IsDead = true;
	}

	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vWindowPos = m_pTarget->Get_Transform()->m_vInfo[INFO_POS];

	vWindowPos.y += 5.f;

	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matView); // 기존 뷰,투영 행렬로 곱
	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matProj);	// 기존 뷰,투영 행렬로 곱

	vWindowPos.x *= WINCX * 0.5f;
	vWindowPos.y *= WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vWindowPos);

	if (static_cast<CMonster*>(m_pTarget)->Detect() && !m_bRecogDetect)
	{
		m_pTransformCom->Set_Scale(_vec3(2.f, 8.f, 1.f));
		m_iRecogTextureIndex = 0;
		m_bRecogDetect = true;
	}
	else if (!m_bRecogDetect)
	{
		m_pTransformCom->Set_Scale(_vec3(6.f, 6.f, 1.f));
		m_iRecogTextureIndex = 1;
	}


	if (m_iRecogTextureIndex == 2 || m_iRecogTextureIndex == 3)
	{
		m_fRecogHitTypeAge += fTimeDelta;
		m_pTransformCom->Set_Scale(_vec3(6.f, 6.f, 1.f));

		if(m_fRecogHitTypeAge > m_fRecogHitTypeLifeTime)
		{
			m_bRecogDetect = false;

			m_fRecogHitTypeAge = 0.f;
		}
	}
}

void CRecognitionRange::TargetUI(const _float& fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(m_pTarget);

	if (pMonster->Get_Info().bDead)
	{
		m_IsDead = true;
	}

	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vWindowPos = m_pTarget->Get_Transform()->m_vInfo[INFO_POS];

	vWindowPos.y += 8.f;

	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matView); // 기존 뷰,투영 행렬로 곱
	D3DXVec3TransformCoord(&vWindowPos, &vWindowPos, &matProj);	// 기존 뷰,투영 행렬로 곱

	vWindowPos.x *= WINCX * 0.5f;
	vWindowPos.y *= WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vWindowPos);
	m_pTransformCom->Set_Scale(_vec3(8.f, 8.f, 1.f));


}

void CRecognitionRange::TextureSet()
{
																													//todo index = 0 Idle상태이고 플레이어를 발견한 상태
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/Sleep1.png", 1);			//todo index = 1 Idle상태이고 플레이어를 발견하지 못한 상태
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/Match.png", 1);			//todo index = 2 히트타입과 총알타입이 일치
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/NoMatch.png", 1);		//todo index = 3 히트타입과 총알타입이 불일치
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/ObjectiveIcon1.png", 1); //todo index = 4 몬스터 위치 표시 UI
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Recog/Destination.png", 1);	//todo index = 5 목적지 위치 표시 UI
}

void CRecognitionRange::DestinationRender()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(5); // 목표지점 UI 인덱스
	m_pBufferCom->Render_Buffer();

	//TODO UI 위쪽에 거리를 표시해주는 폰트를 출력하려했으나 느낌이 오래걸릴 것 같아 다른 작업하러감
	//CFontMgr::GetInstance()->Render_Font(L"MISSION_FONT",to_wstring(m_fDestinationDistance).c_str(),&_vec2(m_vDestinationTextPos.x,m_vDestinationTextPos.y), D3DXCOLOR(D3DCOLOR_ARGB(255, 130, 245, 209)),20,false);
	//Engine::Render_Font(L"DIALOG_FONT", L"로딩 중...", &_vec2(400, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)), 20, false);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRecognitionRange::RecogRender()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);



	m_pTextureCom->Render_Textrue(m_iRecogTextureIndex); // 발견
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRecognitionRange::TargetRender()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(4); //todo index = 4 몬스터 위치 표시 UI
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRecognitionRange::DistanceRender()
{

	_vec3 vTargetPos = m_pTarget->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vPlayerPos = m_pPlayerTransformCom->Get_Transform()->m_vInfo[INFO_POS];

	_vec3 vPlayerToTarget;

	vPlayerToTarget = vPlayerPos - vTargetPos;

	_float fLength = D3DXVec3Length(&vPlayerToTarget);

	if (fLength < m_fRenderDistance)
	{
		m_bDistanceRender = true;
	}
	else
		m_bDistanceRender = false;

}

void CRecognitionRange::AngleRender()
{
	if (m_pFrustum->IsIn(m_pTransformCom->m_vInfo[INFO_POS]))
	{
		m_bAngleRender = true;
	}
	else
		m_bAngleRender = false;

}


CRecognitionRange* CRecognitionRange::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget, UI_TYPE eUIType)
{
	CRecognitionRange* pInstance = new CRecognitionRange(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("RecogUI Create Failed");
		return nullptr;
	}

	pInstance->m_pTarget = pTarget;
	pInstance->m_eUIType = eUIType;

	return pInstance;
}

void Engine::CRecognitionRange::Free()
{
	__super::Free();
}
