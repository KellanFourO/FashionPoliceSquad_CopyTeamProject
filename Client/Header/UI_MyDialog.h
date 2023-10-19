#pragma once
#include "GameObject.h"
#include "UI_Portrait.h"
#include "UI_MissionObjective.h"
#include "UI_NameTag.h"

// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMyDialog : public Engine::CGameObject
{

private:
	explicit					 CMyDialog(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CMyDialog(const CMyDialog& rhs);
	virtual						~CMyDialog();

public:
	virtual HRESULT				Ready_GameObject(DIALOGTAG eDialogTag);
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

public:
	// Get
	DIALOGDATA*					Get_Info() { return m_pDialogInfo; }
	void						Set_EndInput(_bool bEndInput) { m_bEndInput = bEndInput;}
	_bool						Get_Quiz() { return m_bQuiz;}

private:
	HRESULT							Add_Component();
	void							LoadText(DIALOGTAG eDialogTag);
	void							KeyInput();
	void							QuizKeyInput();

private:
	CUITex*							m_pBufferCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;

	DIALOGDATA*						m_pDialogInfo = nullptr;


	_matrix							m_matProj, m_matView;
	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;

	_bool							m_bDialog;
	_bool							m_bLateInit = true;
	_bool							m_bSound = true;
	_bool							m_bRenewal = true;
	_bool							m_bEndInput = false;
	_bool							m_bQuiz = false;
	_bool							m_bQuizInput = false;
	DIALOGTAG						m_eDialog;


	CMissionObjective*				m_pMission = nullptr;
	CPortrait*						m_pPortrait = nullptr;
	CNameTag*						m_pNameTag = nullptr;


	list<PORTRAITTAG>				m_PortraitList;
	list<CNameTag*>					m_NameTagList;
	list<wstring>					m_TextList;


	_float							m_fNextTick = 0;
	_bool							m_bTick = true;



public:
	static CMyDialog*			Create(LPDIRECT3DDEVICE9 pGraphicDev, DIALOGTAG eDialogTag);

private:
	virtual void				Free() override;
};

END


