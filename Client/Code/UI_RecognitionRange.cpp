#include "stdafx.h"
#include "UI_RecognitionRange.h"
#include "UIMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"
#include "Monster.h"


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
// 	D3DXMatrixIdentity(&m_matView);
// 	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f,2.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTextureArray[SEARCH] = CUIMgr::GetInstance()->Get_UI_Clone(L"!.png")->Get_Info()->pTexture;
	m_pTextureArray[NOSEARCH] = CUIMgr::GetInstance()->Get_UI_Clone(L"Sleep.png")->Get_Info()->pTexture;
	m_pTextureArray[MATCH] = CUIMgr::GetInstance()->Get_UI_Clone(L"Match.png")->Get_Info()->pTexture;
	m_pTextureArray[NOMATCH] = CUIMgr::GetInstance()->Get_UI_Clone(L"NoMatch.png")->Get_Info()->pTexture;
	m_pTextureArray[STUN] = CUIMgr::GetInstance()->Get_UI_Clone(L"sturnstar.png")->Get_Info()->pTexture;
	m_pTextureArray[STUNRING] = CUIMgr::GetInstance()->Get_UI_Clone(L"stunring.png")->Get_Info()->pTexture;

	m_pFrontTextureCom->Set_Texture(m_pTextureArray[NOSEARCH], 0);
	return S_OK;
}

Engine::_int Engine::CRecognitionRange::Update_GameObject(const _float& fTimeDelta)
{
	//	TODO - 몬스터의 상태값을 가져온다고 가정한상황 ( 발견, 미발견, 스턴 )
	int i = 1;

	switch (i)
	{
	case 0 :
		m_pFrontTextureCom->Set_Texture(m_pTextureArray[SEARCH], 0);
		m_pBackTextureCom->Set_Texture(nullptr, 0);
		break;
	case 1 :
		m_pFrontTextureCom->Set_Texture(m_pTextureArray[NOSEARCH], 0);
		m_pBackTextureCom->Set_Texture(nullptr, 0);
		break;
	case 2 :
		m_pFrontTextureCom->Set_Texture(m_pTextureArray[MATCH], 0);
		m_pBackTextureCom->Set_Texture(nullptr, 0);
		break;
	case 3 :
		m_pFrontTextureCom->Set_Texture(m_pTextureArray[NOMATCH], 0);
		m_pBackTextureCom->Set_Texture(nullptr, 0);
		break;
	case 4 :
		m_pFrontTextureCom->Set_Texture(m_pTextureArray[STUN], 0);
		m_pBackTextureCom->Set_Texture(m_pTextureArray[STUNRING], 0);
		break;
	case 5 :

		break;
	}

	UI_Translate();

	Engine::Add_RenderGroup(RENDER_UI, this);

	_int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void Engine::CRecognitionRange::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CRecognitionRange::Render_GameObject()
{
	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	matProj._43 = 0;

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
 	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
 	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
//
// 	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//
// 	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
// 	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
// 	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);


	if (m_pBackTextureCom->Get_Texture(0))
	{
		m_pBackTextureCom->Render_Textrue();
		m_pBufferCom->Render_Buffer();
	}

	m_pFrontTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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

	pComponent = m_pFrontTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BaseUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pBackTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BaseUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CRecognitionRange::UI_Translate()
{
	_vec3 vTargetWorldPos;
	vTargetWorldPos = m_pTarget->m_pTransformCom->m_vInfo[INFO_POS]; // todo 아까 보여드린 놈 월드좌표

	_matrix matTargetWorld;
	matTargetWorld = *m_pTarget->m_pTransformCom->Get_WorldMatrix(); // todo 마찬가지 m_pTarget이 몬스터입니다

	_vec3 vPlayerWorldPos;
	vPlayerWorldPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back()->m_pTransformCom->m_vInfo[INFO_POS];

	_matrix matCamView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamView);

	_matrix matWVP;
	matWVP = matTargetWorld * matCamView * m_matProj;

	_vec3 vScreenPos;
	D3DXVec3TransformCoord(&vScreenPos, &vTargetWorldPos, &matWVP);

	vScreenPos = { (vScreenPos.x + 1.0f) * (WINCX * 0.5f), ( 1.0f - vScreenPos.y) * (WINCY * 0.5f), 0.f };

	//TODO vScreenPos가 몬스터 월드 좌표 -> 스크린 좌표로 변환한거임.
	//
	//// !UI가 플레이어를 항상 바라보게 해주는 코드
	//_vec3 vUItoPlayerDir;
	//vUItoPlayerDir = vPlayerWorldPos - m_pTransformCom->m_vInfo[INFO_POS];
	//D3DXVec3Normalize(&vUItoPlayerDir, &vUItoPlayerDir);
	//_float fAngle = atan2f(vUItoPlayerDir.x, vUItoPlayerDir.z);
	//m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
	//// !UI가 플레이어를 항상 바라보게 해주는 코드 종료

	// 스크린 좌표를 기준으로 SetPos를 해라.
	// 현재 스크린 좌표는 X : 425, Y : 278 이다.
	// 이 스크린 좌표를 기준으로 다시 해보자

	// pos.x = pos.x - WINCX * 0.5f = 425 - 400 = 25
	// pos.y = -pos.y(-278) + WINCY * 0.5f = -278 + 300 = 22

	// 25 , 22  를기준으로 뭔가를 할 수 있을까? 완전 중앙 라인으로 잡힌다.

	// 몬스터의 월드위치는 {x=24.8971157 y=4.05144262 z=27.8353844 }
	// 새로운 방법을 강구해야 한다.


	_float fScreenX = vScreenPos.x - WINCX * 0.5f;
	_float fScreenY = -vScreenPos.y + WINCY * 0.5f;

	_float fOffset = m_pTarget->m_pTransformCom->m_vScale.y;



	_vec3 vUIPos, vScale;


	vUIPos = { vTargetWorldPos.x, vTargetWorldPos.y + fOffset, vTargetWorldPos.z };
	vScale = { 10.f, 10.f, 1.f };

	int i = 0;



	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vUIPos);

}



void CRecognitionRange::Sprite_Function(const _float& fTimeDelta)
{
// 	auto& ObjectTEMP = Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER);
// 	CPlayer* pPlayer = nullptr;
//
// 	if (ObjectTEMP.size() != 0)
// 	{
// 		pPlayer = dynamic_cast<CPlayer*>(Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back());
// 		m_pPlayerInfo = pPlayer->Get_INFO();
// 	}
// 	//m_tPlayerInfo.PlayerState
// 	if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Left)
// 	{
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_left.png")->Get_Info()->pTexture, 0);
// 	}
// 	else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Right)
// 	{
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_right.png")->Get_Info()->pTexture, 0);
// 	}
// 	else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Hit)
// 	{
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_front_hurt.png")->Get_Info()->pTexture, 0);
// 	}
// 	else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Heal)
// 	{
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smile.png")->Get_Info()->pTexture, 0);
// 	}
// 	else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Heal)
// 	{
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smirk_low.png")->Get_Info()->pTexture, 0);
// 	}
// 	else
// 	{
// 		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"SPRITE_hud_front.png")->Get_Info()->pTexture, 0);
// 	}

}

void CRecognitionRange::Font_Function(const _float& fTimeDelta)
{
}

void CRecognitionRange::NonTargetUI_Function()
{
// 	auto& ObjectTEMP = Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER);
// 	CPlayer* pPlayer = nullptr;
//
// 	if (ObjectTEMP.size() != 0)
// 	{
// 		pPlayer = dynamic_cast<CPlayer*>(Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back());
// 		m_pPlayerInfo = pPlayer->Get_INFO();
// 	}
//
// 	if (m_eType == UI_TYPE::VALUE)
// 	{
// 		_float fMaxHp = pPlayer->Get_INFO()->fMaxHP;
// 		_float fCurHp = pPlayer->Get_INFO()->fHP;
//
// 		_float fRatio = fCurHp / fMaxHp;
// 		_float fOrigin = m_pInfo->vSize.x;
// 		_float fSecond = fOrigin * fRatio;
//
// 		_float fResult = fOrigin - fSecond;
//
//
// 		m_pBufferCom->SetRatio(fRatio);
//
// 		m_pTransformCom->m_vScale.x = m_pInfo->vSize.x * fRatio;
// 		m_pTransformCom->m_vScale.y = m_pInfo->vSize.y;
// 		m_fX = m_pInfo->vPos.x - fResult;
// 		m_fY = m_pInfo->vPos.y;
// 		m_pTransformCom->m_vInfo[INFO_POS].x = m_fX - WINCX * 0.5f;
// 		m_pTransformCom->m_vInfo[INFO_POS].y = -m_fY + WINCY * 0.5f;
// 	}
// 	else
// 	{
// 		m_pTransformCom->m_vScale.x = m_pInfo->vSize.x;
// 		m_pTransformCom->m_vScale.y = m_pInfo->vSize.y;
// 		m_fX = m_pInfo->vPos.x; // 50
// 		m_fY = m_pInfo->vPos.y; //
// 		m_pTransformCom->m_vInfo[INFO_POS].x = m_fX - WINCX * 0.5f;
// 		m_pTransformCom->m_vInfo[INFO_POS].y = -m_fY + WINCY * 0.5f;
// 	}
//
// 	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
// 	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
// 	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CRecognitionRange::TargetUI_Function()
{
// 	CMonster* pMonster = dynamic_cast<CMonster*>(m_pTarget);
// 	//pMonster->Get_Info().fMaxHP;
// 	//pMonster->Get_Info().fHP;
//
// 	_float fMaxHp = pMonster->Get_Info().fMaxHP;
// 	_float fCurHp = pMonster->Get_Info().fHP;
//
// 	_float fRatio = fCurHp / fMaxHp;
// 	_float fOrigin = 0.5f;
// 	_float fSecond = fOrigin * fRatio;
//
// 	_float fResult = fOrigin - fSecond;
//
// 	m_pBufferCom->SetRatio(fRatio);
//
// 	// 몬스터의 트랜스폼
// 	CTransform* pMonsterTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(ID_DYNAMIC, COMPONENTTAG::TRANSFORM));
//
// 	// 플레이어 트랜스폼
// 	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
//
// 	// 원근투영을 위한 카메라 얻어오기
// 	CNewFPSCamera* pNEWFPSCamera = dynamic_cast<CNewFPSCamera*>(Engine::Management()->Get_Layer(LAYERTAG::CAMERA)->Get_ObjectList(OBJECTTAG::FPSCAMERA).back());
//
// 	// !UI가 플레이어를 항상 바라보게 해주는 코드
// 	_vec3 vUItoPlayerDir;
// 	vUItoPlayerDir = pPlayerTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
// 	D3DXVec3Normalize(&vUItoPlayerDir, &vUItoPlayerDir);
// 	_float fAngle = atan2f(vUItoPlayerDir.x, vUItoPlayerDir.z);
// 	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
// 	// !UI가 플레이어를 항상 바라보게 해주는 코드 종료
//
// 	_vec3 vTargetWorldPos;
// 	pMonsterTransform->Get_Info(INFO_POS, &vTargetWorldPos);
//
// 	m_pTransformCom->m_vScale.x = 0.5f * fRatio;
// 	m_pTransformCom->m_vScale.y = 0.05f;
// 	m_pTransformCom->m_vScale.z = 1.f;
//
// 	m_pTransformCom->m_vInfo[INFO_POS].x = vTargetWorldPos.x + fResult;
// 	m_pTransformCom->m_vInfo[INFO_POS].y = vTargetWorldPos.y + 0.7f;
// 	m_pTransformCom->m_vInfo[INFO_POS].z = vTargetWorldPos.z;
//
// 	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
// 	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pNEWFPSCamera->Get_View());
// 	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pNEWFPSCamera->Get_Proj());
}


CRecognitionRange* CRecognitionRange::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget)
{
	CRecognitionRange* pInstance = new CRecognitionRange(pGraphicDev);

	pInstance->Set_Target(pTarget);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyUI Create Failed");
		return nullptr;
	}



	return pInstance;
}

void Engine::CRecognitionRange::Free()
{
	for (auto iter : m_pTextureArray)
	{
		Safe_Release(iter);
	}

	__super::Free();
}
