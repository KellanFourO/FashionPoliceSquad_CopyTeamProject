#include "stdafx.h"
#include "UI_CardList.h"
#include "EventMgr.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include <random>

CCardList::CCardList(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CCardList::CCardList(const CCardList& rhs)
	: Engine::CGameObject(rhs)
{
}

CCardList::~CCardList()
{
}

HRESULT Engine::CCardList::Ready_GameObject()
{

	switch (m_eDialogTag)
	{
	case Engine::DIALOGTAG::STORY_ST1_DEVELOP:

		for (int i = 0; i < 3; ++i)
		{
			switch (i)
			{
				case (int)CARD_DIR::CARD_LEFT:
				{
					CMyCard* pCard = CMyCard::Create(m_pGraphicDev, 150.f, CARD_DIR::CARD_LEFT, CARD_TYPE::SPEED);
					NULL_CHECK_RETURN(pCard, E_FAIL);
					m_vecCard.push_back(pCard);
					break;
				}

				case (int)CARD_DIR::CARD_CENTER:
				{
					CMyCard* pCard = CMyCard::Create(m_pGraphicDev, 400.f, CARD_DIR::CARD_CENTER, CARD_TYPE::BOMB);
					NULL_CHECK_RETURN(pCard, E_FAIL);
					m_vecCard.push_back(pCard);
					break;
				}

				case (int)CARD_DIR::CARD_RIGHT:
				{

					CMyCard* pCard = CMyCard::Create(m_pGraphicDev, 650.f, CARD_DIR::CARD_RIGHT, CARD_TYPE::INTELLIGENCE);
					NULL_CHECK_RETURN(pCard, E_FAIL);
					m_vecCard.push_back(pCard);
					break;
				}
			}
		}
		break;
	case Engine::DIALOGTAG::STORY_LOBBY_CONCLU:
		for (int i = 0; i < 3; ++i)
		{
			switch (i)
			{
			case (int)CARD_DIR::CARD_LEFT:
			{
				CMyCard* pCard = CMyCard::Create(m_pGraphicDev, 150.f, CARD_DIR::CARD_LEFT, CARD_TYPE::FORCE);
				NULL_CHECK_RETURN(pCard, E_FAIL);
				m_vecCard.push_back(pCard);
				break;
			}

			case (int)CARD_DIR::CARD_CENTER:
			{
				CMyCard* pCard = CMyCard::Create(m_pGraphicDev, 400.f, CARD_DIR::CARD_CENTER, CARD_TYPE::STRENGTH);
				NULL_CHECK_RETURN(pCard, E_FAIL);
				m_vecCard.push_back(pCard);
				break;
			}

			case (int)CARD_DIR::CARD_RIGHT:
			{

				CMyCard* pCard = CMyCard::Create(m_pGraphicDev, 650.f, CARD_DIR::CARD_RIGHT, CARD_TYPE::BONUS);
				NULL_CHECK_RETURN(pCard, E_FAIL);
				m_vecCard.push_back(pCard);
				break;
			}
			}
		}
		break;
	}




	return S_OK;
}

Engine::_int Engine::CCardList::Update_GameObject(const _float& fTimeDelta)
{
	_int IExit;
	__super::Update_GameObject(fTimeDelta);
	KeyInput();


	_int iCount = 0;
	_int iPickingIndex = 0;

	for (int i = 0; i < m_vecCard.size(); ++i)
	{
		IExit = m_vecCard[i]->Update_GameObject(fTimeDelta);

		if(m_vecCard[i]->Get_RealPick())
		iPickingIndex = i;

		if (m_vecCard[i]->Get_Dialog())
		{
			iCount++;

		}
		else
			iCount = 0;

	}

	if (iCount == 3)
	{
		if (m_vecCard[iPickingIndex]->Get_CardType() == CARD_TYPE::FORCE)
		{
			CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::LOBBY, DIALOGTAG::SKILL_DASH);
		}
		else if (m_vecCard[iPickingIndex]->Get_CardType() == CARD_TYPE::SPEED)
		{
			CEventMgr::GetInstance()->OnDialog(m_pGraphicDev, SCENETAG::STAGE, DIALOGTAG::SKILL_ROPE);
		}
	}


	if (m_bPicking)
		m_iLateTick++;



	return IExit;
}

void Engine::CCardList::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

	for (int i = 0; i < m_vecCard.size(); ++i)
	{
		m_vecCard[i]->LateUpdate_GameObject();
	}

	if (m_iLateTick > 120)
	{
		for (int i = 0; i < m_vecCard.size(); ++i)
		{
			if (!m_vecCard[i]->Get_Picking())
				m_vecCard[i]->Set_Picking(true);
		}
	}

}

void CCardList::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_F1) & 0x80)
	{
		m_vecCard[(int)CARD_DIR::CARD_LEFT]->Set_Picking(TRUE);
		m_vecCard[(int)CARD_DIR::CARD_LEFT]->Set_RealPick(TRUE);
		m_bPicking = TRUE;
	}

	else if (Engine::Get_DIKeyState(DIK_F2) & 0x80)
	{
		m_vecCard[(int)CARD_DIR::CARD_CENTER]->Set_Picking(TRUE);
		m_vecCard[(int)CARD_DIR::CARD_CENTER]->Set_RealPick(TRUE);
		m_bPicking = TRUE;
	}

	else if (Engine::Get_DIKeyState(DIK_F3) & 0x80)
	{
		m_vecCard[(int)CARD_DIR::CARD_RIGHT]->Set_Picking(TRUE);
		m_vecCard[(int)CARD_DIR::CARD_RIGHT]->Set_RealPick(TRUE);
		m_bPicking = TRUE;
	}
}

void CCardList::Render_GameObject()
{

}

CCardList* CCardList::Create(LPDIRECT3DDEVICE9 pGraphicDev, DIALOGTAG eDialogTag)
{
	CCardList* pInstance = new CCardList(pGraphicDev);
	pInstance->m_eDialogTag = eDialogTag;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CardList Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Engine::CCardList::Free()
{
	__super::Free();
}
