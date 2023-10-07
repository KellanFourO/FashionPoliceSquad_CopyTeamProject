#include "stdafx.h"
#include "MyUI.h"
#include "ImGuiManager.h"
#include "UIMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"

#include "Player.h"
#include "Monster.h"
#include "NewFPSCamera.h"

#include <DirectXMath.h>

CMyUI::CMyUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CMyUI::CMyUI(const CMyUI& rhs)
	: CGameObject(rhs)
{
	m_pInfo = rhs.m_pInfo;
	m_eType = rhs.m_eType;

	m_matView = rhs.m_matView;
	m_matProj = rhs.m_matProj;

	m_pTextureCom = rhs.m_pTextureCom;
	m_pTransformCom = rhs.m_pTransformCom;
	m_pBufferCom = rhs.m_pBufferCom;
}

CMyUI::~CMyUI()
{
}

HRESULT Engine::CMyUI::Ready_GameObject(IDirect3DBaseTexture9* pTexture)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pInfo = new UIDATA;
	m_pInfo->vPos = { 100.f, 100.f, 0.f };
	m_pInfo->vSize = { 100.f, 100.f, 1.f };
	m_pInfo->pTexture = pTexture;
	m_pTextureCom->Set_Texture(m_pInfo->pTexture, 0);



	return S_OK;
}

Engine::_int Engine::CMyUI::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);

	if (m_bLateInit)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back());

		if (m_pPlayer->Get_Gun())
		{
			m_pPlayerGun = m_pPlayer->Get_Gun();
			m_bLateInit = false;
		}
	}

	//if (CImGuiManager::GetInstance()->Get_vecUI().size() != 0)
	//{
	//	m_pInfo = CImGuiManager::GetInstance()->Get_ImguiUI()->Get_Info();
	//	m_pTextureCom->Set_Texture(m_pInfo->pTexture, 0);
	//
	//}

	switch (m_eType)
	{

	case Engine::UI_TYPE::HP:
		//HP_Function(fTimeDelta);
		break;

	case Engine::UI_TYPE::SPRITE:
		Sprite_Function(fTimeDelta);
		break;

	case Engine::UI_TYPE::FONT:
		Font_Function(fTimeDelta);
		break;

	case Engine::UI_TYPE::BASIC:
		break;
	}

	// 1행 너비스케일
	// 2행 업스케일
	// 3행 깊이스케일

	_int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void Engine::CMyUI::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMyUI::Render_GameObject()
{
	if (!m_bClone) // 타겟 UI같은경우에는 클론을 하는 경우가 많아서 클론인지 아닌지로 구분
	{
		NonTargetUI_Function();
	}
	else
	{
		if (m_pTarget)
		{
			TargetUI_Function();
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();

	Bullet_Function();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CMyUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BaseUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMyUI::HP_Function(const _float& fTimeDelta)
{
	// 플레이어의 체력 정보를 가지고 왔다고 가정.

	float fMaxHp = m_pPlayer->Get_INFO()->fMaxHP;
	float fCurrentHp = m_pPlayer->Get_INFO()->fHP;

	float fHpRatio = fCurrentHp / fMaxHp;

	dynamic_cast<CUITex*>(m_pBufferCom)->SetRatio(fHpRatio);

}

void CMyUI::Sprite_Function(const _float& fTimeDelta)
{

	//m_tPlayerInfo.PlayerState
	//if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Left)
	//{
	//	m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_left.png")->Get_Info()->pTexture, 0);
	//}
	//else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Right)
	//{
	//	m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_right.png")->Get_Info()->pTexture, 0);
	//}
	//else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Hit)
	//{
	//	m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_front_hurt.png")->Get_Info()->pTexture, 0);
	//}
	//else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Heal)
	//{
	//	m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smile.png")->Get_Info()->pTexture, 0);
	//}
	//else if (m_pPlayerInfo->PlayerState->StateID == PlayerStateID::Player_Heal)
	//{
	//	m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smirk_low.png")->Get_Info()->pTexture, 0);
	//}
	//else
	//{
	//	m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"SPRITE_hud_front.png")->Get_Info()->pTexture, 0);
	//}





	/*if (iCount < 10)
	{
		++iCount;
	}
	else
	{
		++iTempState;
		iCount = 0;
	}

	if (iTempState > 12)
		iTempState = 0;



	switch (iTempState)
	{
	case 0:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"SPRITE_hud_front.png")->Get_Info()->pTexture, 0);
		break;

	case 1:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_front_hurt.png")->Get_Info()->pTexture, 0);
		break;

	case 2:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_grimace_low.png")->Get_Info()->pTexture, 0);
		break;

	case 3:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_left.png")->Get_Info()->pTexture, 0);
		break;

	case 4:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_right.png")->Get_Info()->pTexture, 0);
		break;

	case 5:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smile.png")->Get_Info()->pTexture, 0);
		break;

	case 6:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smirk.png")->Get_Info()->pTexture, 0);
		break;

	case 7:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_sad_dirt.png")->Get_Info()->pTexture, 0);
		break;

	case 8:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_lef_hurt.png")->Get_Info()->pTexture, 0);
		break;

	case 9:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_grimace.png")->Get_Info()->pTexture, 0);
		break;

	case 10:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_left_hurt.png")->Get_Info()->pTexture, 0);
		break;

	case 11:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smirk_low.png")->Get_Info()->pTexture, 0);
		break;

	case 12:
		m_pTextureCom->Set_Texture(CUIMgr::GetInstance()->Get_UI(L"des-hud_smile_low.png")->Get_Info()->pTexture, 0);
		break;

	}*/

}

void CMyUI::Font_Function(const _float& fTimeDelta)
{
}

void CMyUI::Bullet_Function()
{

	if (m_eType == UI_TYPE::BULLET && m_pPlayerGun)
	{
		Engine::Render_Font(L"UI_WEAPON_NAME", m_pPlayerGun->Get_GunInfo()->m_szGunName.c_str(), &_vec2(640, 520), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)));
		Engine::Render_Font(L"UI_FONT", to_wstring(m_pPlayerGun->Get_GunInfo()->m_iCurrentBullet).c_str(), &_vec2(670, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 130, 245, 209)));
		Engine::Render_Font(L"UI_FONT", to_wstring(m_pPlayerGun->Get_GunInfo()->m_iMaxBullet).c_str(), &_vec2(730, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 130, 245, 209)));
	}
}

void CMyUI::NonTargetUI_Function()
{
	auto& ObjectTEMP = Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER);
	CPlayer* pPlayer = nullptr;

	if (ObjectTEMP.size() != 0)
	{
		pPlayer = dynamic_cast<CPlayer*>(Engine::Management()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).back());
		m_pPlayerInfo = pPlayer->Get_INFO();
	}

	if (m_eType == UI_TYPE::VALUE)
	{
		_float fMaxHp = pPlayer->Get_INFO()->fMaxHP;
		_float fCurHp = pPlayer->Get_INFO()->fHP;

		_float fRatio = fCurHp / fMaxHp;
		_float fOrigin = m_pInfo->vSize.x;
		_float fSecond = fOrigin * fRatio;

		_float fResult = fOrigin - fSecond;


		m_pBufferCom->SetRatio(fRatio);

		m_pTransformCom->m_vScale.x = m_pInfo->vSize.x * fRatio;
		m_pTransformCom->m_vScale.y = m_pInfo->vSize.y;
		m_fX = m_pInfo->vPos.x - fResult;
		m_fY = m_pInfo->vPos.y;
		m_pTransformCom->m_vInfo[INFO_POS].x = m_fX - WINCX * 0.5f;
		m_pTransformCom->m_vInfo[INFO_POS].y = -m_fY + WINCY * 0.5f;
	}
	else
	{
		m_pTransformCom->m_vScale.x = m_pInfo->vSize.x;
		m_pTransformCom->m_vScale.y = m_pInfo->vSize.y;
		m_fX = m_pInfo->vPos.x; // 50
		m_fY = m_pInfo->vPos.y; //
		m_pTransformCom->m_vInfo[INFO_POS].x = m_fX - WINCX * 0.5f;
		m_pTransformCom->m_vInfo[INFO_POS].y = -m_fY + WINCY * 0.5f;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CMyUI::TargetUI_Function()
{
	CMonster* pMonster = dynamic_cast<CMonster*>(m_pTarget);
	//pMonster->Get_Info().fMaxHP;
	//pMonster->Get_Info().fHP;

	_float fMaxHp = pMonster->Get_Info().fMaxHP;
	_float fCurHp = pMonster->Get_Info().fHP;

	_float fRatio = fCurHp / fMaxHp;
	_float fOrigin = 4.f;
	_float fSecond = fOrigin * fRatio;

	_float fResult = fOrigin - fSecond;

	m_pBufferCom->SetRatio(fRatio);

	// 몬스터의 트랜스폼
	CTransform* pMonsterTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(ID_DYNAMIC, COMPONENTTAG::TRANSFORM));

	// 플레이어 트랜스폼
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	// 원근투영을 위한 카메라 얻어오기
	CNewFPSCamera* pNEWFPSCamera = dynamic_cast<CNewFPSCamera*>(Engine::Management()->Get_Layer(LAYERTAG::CAMERA)->Get_ObjectList(OBJECTTAG::FPSCAMERA).back());

	// !UI가 플레이어를 항상 바라보게 해주는 코드
	_vec3 vUItoPlayerDir;
	vUItoPlayerDir = pPlayerTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vUItoPlayerDir, &vUItoPlayerDir);
	_float fAngle = atan2f(vUItoPlayerDir.x, vUItoPlayerDir.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
	// !UI가 플레이어를 항상 바라보게 해주는 코드 종료

	_vec3 vTargetWorldPos;
	pMonsterTransform->Get_Info(INFO_POS, &vTargetWorldPos);

	m_pTransformCom->m_vScale.x = 4.f * fRatio;
	m_pTransformCom->m_vScale.y = 0.25f;
	m_pTransformCom->m_vScale.z = 1.f;

	m_pTransformCom->m_vInfo[INFO_POS].x = vTargetWorldPos.x + fResult;
	m_pTransformCom->m_vInfo[INFO_POS].y = vTargetWorldPos.y + 3.f;
	m_pTransformCom->m_vInfo[INFO_POS].z = vTargetWorldPos.z;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pNEWFPSCamera->Get_View());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pNEWFPSCamera->Get_Proj());
}


CMyUI* CMyUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, IDirect3DBaseTexture9* pTexture, UI_TYPE eType)
{
	CMyUI* pInstance = new CMyUI(pGraphicDev);
	pInstance->m_eType = eType;

	if (FAILED(pInstance->Ready_GameObject(pTexture)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyUI Create Failed");
		return nullptr;
	}



	return pInstance;
}

void Engine::CMyUI::Free()
{
	Safe_Delete(m_pInfo);
	__super::Free();
}
