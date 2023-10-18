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

	void	OnLevelUp(LPDIRECT3DDEVICE9 pGraphicDev,SCENETAG eSceneTag); // 카드 생성 및 기타 이벤트
	void	OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, DIALOGTAG eDialogTag); // 다이얼로그 생성
	void	OnPause(_bool bPause, SCENETAG eSceneTag);

	enum class MiniGameState { PLAY_NOW, NOT_PLAY };
	//유진 추가
	HRESULT		OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	HRESULT		OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	HRESULT		OnMiniGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);

	HRESULT		OffMiniGame_Arrow(SCENETAG eSceneTag, _bool ClearCheck);
	HRESULT		OffMiniGame_KickBoard(SCENETAG eSceneTag, _bool ClearCheck);
	HRESULT		OffMiniGame_Quiz(SCENETAG eSceneTag, _bool ClearCheck);
													//True가 클리어

	_bool		Get_MiniGameMode() { return m_bMiniGame_Mode; }
	void		Set_MiniGameMode() { m_bMiniGame_Mode == true; }

	_bool		Get_MiniGameClearCheck(_uint iNumber) { return m_bMiniGame_ClearCheck[iNumber]; }
	void		Set_MiniGameClearCheck(_uint iNumber, _bool _TrueOrFalse) { m_bMiniGame_ClearCheck[iNumber] = _TrueOrFalse; }

	MiniGameState	Get_MiniGameState() { return m_eMiniGameState; }

private:
	_bool					m_bMiniGame_Mode;
	_bool					m_bMiniGame_ClearCheck[3] = { false, false, false }; //클리어 여부 체크

	MiniGameState			m_eMiniGameState = MiniGameState::NOT_PLAY;

	CMainGame_Arrow*		pGame_Arrow = nullptr;
	CMainGame_KickBoard*	pGame_KickBoard = nullptr;

private:
	virtual		void	Free();
};

END