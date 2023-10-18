#pragma once
#include "Engine_Define.h"
#include "Base.h"

#include "UI_CardList.h"
#include "UI_MyDialog.h"

#include "MainGame_Arrow.h"
#include "MainGame_KickBoard.h"

BEGIN(Engine)

class CEventMgr : public CBase
{
	DECLARE_SINGLETON(CEventMgr)

private:
	explicit CEventMgr();
	virtual ~CEventMgr();

public:
	void    GetEventColider();

	void	OnLevelUp(LPDIRECT3DDEVICE9 pGraphicDev,SCENETAG eSceneTag); // ī�� ���� �� ��Ÿ �̺�Ʈ
	void	OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, DIALOGTAG eDialogTag); // ���̾�α� ����
	void	OnPause(_bool bPause, SCENETAG eSceneTag);

	enum class MiniGameState { PLAY_NOW, NOT_PLAY };
	//���� �߰�
	HRESULT		OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	HRESULT		OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	HRESULT		OnMiniGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);

	HRESULT		OffMiniGame_Arrow(SCENETAG eSceneTag, _bool ClearCheck);
	HRESULT		OffMiniGame_KickBoard(SCENETAG eSceneTag, _bool ClearCheck);
	HRESULT		OffMiniGame_Quiz(SCENETAG eSceneTag, _bool ClearCheck);
													//True�� Ŭ����

	_bool		Get_MiniGameMode() { return m_bMiniGame_Mode; }
	void		Set_MiniGameMode() { m_bMiniGame_Mode == true; }

	_bool		Get_MiniGameClearCheck(_uint iNumber) { return m_bMiniGame_ClearCheck[iNumber]; }
	void		Set_MiniGameClearCheck(_uint iNumber, _bool _TrueOrFalse) { m_bMiniGame_ClearCheck[iNumber] = _TrueOrFalse; }

	MiniGameState	Get_MiniGameState() { return m_eMiniGameState; }

private:
	_bool					m_bMiniGame_Mode;
	_bool					m_bMiniGame_ClearCheck[3] = { false, false, false }; //Ŭ���� ���� üũ

	MiniGameState			m_eMiniGameState = MiniGameState::NOT_PLAY;

	CMainGame_Arrow*		pGame_Arrow = nullptr;
	CMainGame_KickBoard*	pGame_KickBoard = nullptr;

private:
	virtual		void	Free();
};

END