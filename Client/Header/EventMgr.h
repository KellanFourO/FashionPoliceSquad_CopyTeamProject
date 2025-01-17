#pragma once
#include "Engine_Define.h"
#include "Base.h"

#include "UI_CardList.h"
#include "UI_MyDialog.h"
#include "Item.h"

#include "MiniGameResult.h"

#include "MainGame_Arrow.h"
#include "MainGame_KickBoard.h"
#include "MainGame_Quiz.h"

BEGIN(Engine)

class CEventMgr : public CBase
{
	DECLARE_SINGLETON(CEventMgr)

private:
	explicit CEventMgr();
	virtual ~CEventMgr();

public:
	void    GetEventColider();

	void	OnCard(LPDIRECT3DDEVICE9 pGraphicDev,SCENETAG eSceneTag, DIALOGTAG eDialogTag); // 카드 생성 및 기타 이벤트
	void	OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, DIALOGTAG eDialogTag); // 다이얼로그 생성
	void	OnPause(_bool bPause, SCENETAG eSceneTag);
	void	OnDropItem(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, _int iCount);
	void	OffDialog();
	void	SceneChange(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);

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

	_bool		Get_MiniGameLoseCheck(_uint iNumber) { return m_bMiniGame_LoseCheck[iNumber]; }
	void		Set_MiniGameLoseCheck(_uint iNumber, _bool _TrueOrFalse) { m_bMiniGame_LoseCheck[iNumber] = _TrueOrFalse; }

	MiniGameState	Get_MiniGameState() { return m_eMiniGameState; }

	void		Call_MiniGameResult(_int iTimeSec, _int TexNum);

private:


	_bool					m_bMiniGame_Mode;
	_bool					m_bMiniGame_ClearCheck[3] = { false, false, false }; //클리어 여부 체크
	_bool					m_bMiniGame_LoseCheck[3] = { false, false, false }; //졌는지 여부 체크

	MiniGameState			m_eMiniGameState = MiniGameState::NOT_PLAY;

	CMainGame_Arrow*		m_pGame_Arrow = nullptr;
	CMainGame_KickBoard*	m_pGame_KickBoard = nullptr;
	CMainGame_Quiz*			m_pGame_Quiz = nullptr;

	CMiniGameResult*		m_pMiniGameResult = nullptr;
	CMyDialog*				m_pDialog = nullptr;


private:
	virtual		void	Free();
};

END