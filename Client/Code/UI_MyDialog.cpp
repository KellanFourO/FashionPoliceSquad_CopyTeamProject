#include "stdafx.h"
#include "UI_MyDialog.h"
#include <commdlg.h>
#include <Shlwapi.h>
#include <sstream>
#include <utility>
#include "UI_MissionObjective.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Stage1Boss.h"


CMyDialog::CMyDialog(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CMyDialog::CMyDialog(const CMyDialog& rhs)
	: Engine::CGameObject(rhs)
{
}

CMyDialog::~CMyDialog()
{
}



HRESULT Engine::CMyDialog::Ready_GameObject(DIALOGTAG eDialogTag)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPortrait = CPortrait::Create(m_pGraphicDev);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	vPos = { 400.f, 50.f, 0.f };
	vScale = { 359.f * fMultiply, 81.f * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	m_bDialog = true;
	LoadText(eDialogTag);

	m_pPortrait->Set_PortraitTag(m_PortraitList.front());
	return S_OK;

}

Engine::_int Engine::CMyDialog::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);
	_int iExit = __super::Update_GameObject(fTimeDelta);



	m_fNextTick += fTimeDelta;



	if (m_fNextTick >= 2.5f)
	{
		m_bTick = true;
		m_fNextTick = 0.f;
	}

	KeyInput();


	m_pPortrait->Update_GameObject(fTimeDelta);
	return iExit;
}

void Engine::CMyDialog::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	m_pPortrait->LateUpdate_GameObject();
}

void CMyDialog::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();

	//vPos = { 400.f, 50.f, 0.f };
	//vScale = { 359.f * fMultiply, 81.f * fMultiply, 1.f };
	//
	//vPos.x = vPos.x - WINCX * 0.5f;
	//vPos.y = -vPos.y + WINCY * 0.5f;
	if(m_TextList.size() != 0)
	Engine::Render_Font(L"DIALOG_FONT", m_TextList.front().c_str(), &_vec2(250, 40), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)));

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CMyDialog::LoadText(DIALOGTAG eDialogTag)
{
	//std::locale::global(std::locale("korean"));
	wifstream fin;

	switch (eDialogTag)
	{
	case Engine::DIALOGTAG::STORY_ST1_INTRO:
		fin.open(L"../Bin/Data/UI/STORY_ST1_INTRO.dat");
		break;
	case Engine::DIALOGTAG::STORY_ST1_DEVELOP:
		fin.open(L"../Bin/Data/UI/STORY_ST1_DEVELOP.dat");
		break;
	case Engine::DIALOGTAG::STORY_ST1_TURN:
		fin.open(L"../Bin/Data/UI/STORY_ST1_TURN.dat");
		break;
	case Engine::DIALOGTAG::STORY_ST1_CONCLU:
		fin.open(L"../Bin/Data/UI/STORY_ST1_CONCLU.dat");
		break;
	case Engine::DIALOGTAG::QUEST_1:
		fin.open(L"../Bin/Data/UI/QUEST_ST1.dat");
		break;
	case Engine::DIALOGTAG::QUEST_2:
		fin.open(L"../Bin/Data/UI/QUEST_ST2.dat");
		break;
	case Engine::DIALOGTAG::ST1_BOSS_START:
		fin.open(L"../Bin/Data/UI/ST1_BOSS_START.dat");
	}


	if (!fin.fail())
	{
		fin.imbue(std::locale("ko_KR.UTF-8"));

		wstring line;

		while (getline(fin, line)) // 한 줄씩 읽기
		{
			wistringstream iss(line);
			wstring token;

			int iValue = 0;

			_tchar szKey[MAX_PATH] = L"";
			_tchar szText[MAX_PATH] = L"";

			getline(iss, token, L','); // ','를 구분자로 사용하여 값을 자름
			_tcscpy_s(szKey, MAX_PATH, token.c_str());

			getline(iss, token, L',');
			_tcscpy_s(szText, MAX_PATH, token.c_str());
			iValue = _ttoi(szKey);
			PORTRAITTAG readPortrait = static_cast<PORTRAITTAG>(iValue);
			m_PortraitList.push_back(readPortrait);
			m_TextList.push_back(szText);

		}
	}
	fin.close();
}

void CMyDialog::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80 && m_bTick && m_PortraitList.size() != 0 && m_TextList.size() != 0)
	{
		m_PortraitList.pop_front();
		m_TextList.pop_front();

			if (m_PortraitList.size() == 0 || m_TextList.size() == 0)
			{
				if (m_eDialog == DIALOGTAG::QUEST_1 || m_eDialog == DIALOGTAG::QUEST_2)
				{
					CMissionObjective* pMission = dynamic_cast<CMissionObjective*>(Management()->Get_ObjectList(LAYERTAG::UI, OBJECTTAG::MISSION).back());
					pMission->Set_Title(L"QUEST_1");
					pMission->Set_Objective(L"SR 팀과제를 완료하라");
				}

				if (Management()->Get_Scene()->Get_Pause())
				{
					Management()->Get_Scene()->Set_Pause(false);
				}

				m_IsDead = true;
				m_pPortrait->Set_Dead(true);
			}
			else
			{
				m_pPortrait->Set_PortraitTag(m_PortraitList.front());
				m_bTick = false;
			}

	}
}


HRESULT Engine::CMyDialog::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DialogTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}



CMyDialog* CMyDialog::Create(LPDIRECT3DDEVICE9 pGraphicDev, DIALOGTAG eDialogTag)
{
	CMyDialog* pInstance = new CMyDialog(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eDialogTag)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyDialog Create Failed");
		return nullptr;
	}

	pInstance->m_eDialog = eDialogTag;

	return pInstance;
}

void Engine::CMyDialog::Free()
{

	__super::Free();

}
