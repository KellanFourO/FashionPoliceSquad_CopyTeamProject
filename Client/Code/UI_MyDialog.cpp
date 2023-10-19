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
#include "EventMgr.h"

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
	m_pNameTag = CNameTag::Create(m_pGraphicDev);

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
	m_pNameTag->Set_PortraitTag(m_PortraitList.front());

	return S_OK;
}

Engine::_int Engine::CMyDialog::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);
	_int iExit = __super::Update_GameObject(fTimeDelta);

	if (m_bLateInit)
	{
		m_pMission = dynamic_cast<CMissionObjective*>(Management()->Get_ObjectList(LAYERTAG::UI, OBJECTTAG::MISSION).back());
		m_bLateInit = false;
	}

	m_fNextTick += fTimeDelta;


	if (m_fNextTick >= 1.5f)
	{
		m_bTick = true;
		SoundMgr()->StopSound(SOUND_DIALOG);
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
	if (m_TextList.size() != 0)
	{
			Engine::Render_Font(L"TEST_FONT", m_TextList.front().c_str(), &_vec2(250, 40), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)), 20, true);
	}

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
	case Engine::DIALOGTAG::STORY_ST1_CONCLU:
		fin.open(L"../Bin/Data/UI/STORY_ST1_CONCLU.dat");
		break;
	case Engine::DIALOGTAG::STORY_LOBBY_INTRO:
		fin.open(L"../Bin/Data/UI/STORY_LOBBY_INTRO.dat");
		break;
	case Engine::DIALOGTAG::STORY_LOBBY_GAME1:
		fin.open(L"../Bin/Data/UI/STORY_LOBBY_GAME1.dat");
		break;
	case Engine::DIALOGTAG::STORY_LOBBY_GAME2:
		fin.open(L"../Bin/Data/UI/STORY_LOBBY_GAME2.dat");
		break;
	case Engine::DIALOGTAG::STORY_LOBBY_CONCLU:
		fin.open(L"../Bin/Data/UI/STORY_LOBBY_CONCLU.dat");
		break;
	case Engine::DIALOGTAG::ST1_BOSS_INTRO:
		fin.open(L"../Bin/Data/UI/ST1_BOSS_INTRO.dat");
		break;
	case Engine::DIALOGTAG::ST1_BOSS_CONCLU:
		fin.open(L"../Bin/Data/UI/ST1_BOSS_CONCLU.dat");
		break;
	case Engine::DIALOGTAG::STORY_ST2_INTRO:
		fin.open(L"../Bin/Data/UI/STORY_ST2_INTRO.dat");
		break;
	case Engine::DIALOGTAG::STORY_ST2_CONCLU:
		fin.open(L"../Bin/Data/UI/STORY_ST2_CONCLU.dat");
		break;
	case Engine::DIALOGTAG::SKILL_DASH:
		fin.open(L"../Bin/Data/UI/SKILL_DASH.dat");
		break;
	case Engine::DIALOGTAG::SKILL_ROPE:
		fin.open(L"../Bin/Data/UI/SKILL_ROPE.dat");
		break;
	case Engine::DIALOGTAG::ENCOUNTER_DULLSUIT:
		fin.open(L"../Bin/Data/UI/ENCOUNT_DULLSUIT.dat");
		break;
	case Engine::DIALOGTAG::ENCOUNTER_BIGDADDY:
		fin.open(L"../Bin/Data/UI/ENCOUNT_BIGDADDY.dat");
		break;
	case Engine::DIALOGTAG::ENCOUNTER_KICKBOARD:
		fin.open(L"../Bin/Data/UI/ENCOUNT_KICKBOARD.dat");
		break;

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

			getline(iss, token, L'|'); // ','를 구분자로 사용하여 값을 자름
			_tcscpy_s(szKey, MAX_PATH, token.c_str());

			getline(iss, token, L'|');
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
		SoundMgr()->StopSound(SOUND_DIALOG2);

		if (m_eDialog == DIALOGTAG::ST1_BOSS_INTRO && m_bSound)
		{
			SoundMgr()->PlaySoundW(L"PlayerCough.wav", SOUND_DIALOG2, 1.f);
			m_bSound = false;
		}

		if (m_eDialog == DIALOGTAG::STORY_ST1_DEVELOP && m_bRenewal)
		{
			if (m_PortraitList.size() == 5)
			{
				CGameObject* pGameObject = CRecognitionRange::Create(m_pGraphicDev,nullptr,UI_TYPE::DESTINATION);
				static_cast<CRecognitionRange*>(pGameObject)->Set_TargetPos(_vec3(206.62f, 15.f, 246.38f));
				Management()->Get_Scene()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::UI, pGameObject);

				m_bRenewal = false;
			}
		}

		m_PortraitList.pop_front();
		m_TextList.pop_front();


		if (!m_PortraitList.empty())
		{
			switch (m_PortraitList.front())
			{
			case PORTRAITTAG::PORT_DES:
				SoundMgr()->PlaySoundW(L"Player_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_DESDULL:
				SoundMgr()->PlaySoundW(L"Player_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_HALEY:
				SoundMgr()->PlaySoundW(L"Haley_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_HACKER:
				SoundMgr()->PlaySoundW(L"Hacker_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_HEADQUATER:
				SoundMgr()->PlaySoundW(L"HeadQuater_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_MYSTERY:
				SoundMgr()->PlaySoundW(L"Mystery_Sound.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_PHONE:
				SoundMgr()->PlaySoundW(L"Phone_Sound.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_TURNCOAT:
				SoundMgr()->PlaySoundW(L"TurnCoat_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;

			case PORTRAITTAG::PORT_BAUSS:
				SoundMgr()->PlaySoundW(L"Boss_Mumbo.wav", SOUND_DIALOG2, 1.f);
				break;
			}
		}


		SoundMgr()->PlaySoundW(L"DialogEnter2.mp3", SOUND_DIALOG, 1000);



		if (m_PortraitList.size() == 0 || m_TextList.size() == 0 || m_bEndInput)
		{
			switch (m_eDialog)
			{
			case Engine::DIALOGTAG::STORY_ST1_INTRO:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"JS 아카데미의\n범죄자들을 소탕하라");
				break;
			case Engine::DIALOGTAG::STORY_ST1_DEVELOP:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"목적지로 가자");
				CEventMgr::GetInstance()->OnCard(m_pGraphicDev, SCENETAG::STAGE, DIALOGTAG::STORY_ST1_DEVELOP);
				break;
			case Engine::DIALOGTAG::STORY_ST1_CONCLU:
				break;
			case Engine::DIALOGTAG::STORY_LOBBY_INTRO:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"장애물을 돌파하라");
				break;
			case Engine::DIALOGTAG::STORY_LOBBY_GAME1:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"시간내에 암호를 해제하라");
				break;
			case Engine::DIALOGTAG::STORY_LOBBY_GAME2:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"적들을 피해 보급품을 회수하라");
				break;
			case Engine::DIALOGTAG::STORY_LOBBY_CONCLU:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"악의 근원을 퇴치하라");
				CEventMgr::GetInstance()->OnCard(m_pGraphicDev, SCENETAG::LOBBY, DIALOGTAG::STORY_LOBBY_CONCLU);
				break;
			case Engine::DIALOGTAG::ST1_BOSS_INTRO:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"악의 근원을 퇴치하라");
				break;
			case Engine::DIALOGTAG::ST1_BOSS_CONCLU:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"밖으로 나가자");
				break;
			case Engine::DIALOGTAG::STORY_ST2_INTRO:
				m_pMission->Set_Title(L"QUEST");
				m_pMission->Set_Objective(L"범죄자들을 퇴치하라");
				CEventMgr::GetInstance()->OnDropItem(m_pGraphicDev, SCENETAG::STAGE2, 30);
				break;
			case Engine::DIALOGTAG::STORY_ST2_CONCLU:
				m_pMission->Set_Title(L"END");
				m_pMission->Set_Objective(L"임무 완료");
				static_cast<CStage2*>(Management()->Get_Scene())->Set_Video(false);
				break;

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
