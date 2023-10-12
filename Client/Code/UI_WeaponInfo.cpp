#include "stdafx.h"
#include "UI_WeaponInfo.h"
#include "Export_Utility.h"
#include "Export_System.h"



CWeaponInfo::CWeaponInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CWeaponInfo::CWeaponInfo(const CWeaponInfo& rhs)
	: Engine::CGameObject(rhs)
{
}

CWeaponInfo::~CWeaponInfo()
{
}

HRESULT Engine::CWeaponInfo::Ready_GameObject()
{



	UIDATA tData;
	tData.vPos = { 750.f, 300.f, 0.f };
	tData.vSize = { 48.f, 24.f, 1.f };

	CWeaponIcon* pIcon = CWeaponIcon::Create(m_pGraphicDev, tData, 0);
	pIcon->Set_Select(true);
	m_vecWeaponIcon.push_back(pIcon);


	tData.vPos = { 750.f, 350.f, 0.f };
	CWeaponIcon* pIcon2 = CWeaponIcon::Create(m_pGraphicDev, tData, 1);
	m_vecWeaponIcon.push_back(pIcon2);

	m_pPlayer = Management()->Get_Player();
	return S_OK;
}

Engine::_int Engine::CWeaponInfo::Update_GameObject(const _float& fTimeDelta)
{

// 	if (m_bLateInit)
// 	{
// 		m_pPlayer = dynamic_cast<CPlayer*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::PLAYER).back());
// 		m_bLateInit = false;
// 	}

	_int iExit = __super::Update_GameObject(fTimeDelta);

		switch (m_pPlayer->Get_INFO()->Player_GunType)
		{
			case PLAYER_GUNTYPE::SHOTGUN:
			{
				m_vecWeaponIcon[1]->Set_Select(false);
				m_vecWeaponIcon[0]->Set_Select(true);
				m_iSelectIndex = 0;

				break;
			}

			case PLAYER_GUNTYPE::ASSERTRIFLE:
			{
				m_vecWeaponIcon[0]->Set_Select(false);
				m_vecWeaponIcon[1]->Set_Select(true);
				m_iSelectIndex = 1;
				break;
			}
		}


		for (int i = 0; i < m_vecWeaponIcon.size(); ++i)
		{
			m_vecWeaponIcon[i]->Update_GameObject(fTimeDelta);
		}

	return 0;
}

void Engine::CWeaponInfo::LateUpdate_GameObject()
{
	for (int i = 0; i < m_vecWeaponIcon.size(); ++i)
	{
		m_vecWeaponIcon[i]->LateUpdate_GameObject();
	}

	__super::LateUpdate_GameObject();
}

void CWeaponInfo::Render_GameObject()
{
}



CWeaponInfo* CWeaponInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeaponInfo* pInstance = new CWeaponInfo(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("WeaponInfo Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CWeaponInfo::Free()
{
	for (int i = 0; i < m_vecWeaponIcon.size(); ++i)
	{
		Safe_Release(m_vecWeaponIcon[i]);
	}
	__super::Free();
}
