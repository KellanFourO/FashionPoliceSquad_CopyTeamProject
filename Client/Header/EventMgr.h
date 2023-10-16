#pragma once
#include "Engine_Define.h"
#include "Base.h"
#include "UI_CardList.h"
#include "UI_MyDialog.h"
#include "MainGame_Arrow.h"

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


	//유진 추가
	void	OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	void	OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	void	OnMiniGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);

	void	OffMiniGame_Arrow(SCENETAG eSceneTag);
	void	OffMiniGame_KickBoard(SCENETAG eSceneTag);
	void	OffMiniGame_Quiz(SCENETAG eSceneTag);

	_bool	Get_MiniGameMode() { return m_bMiniGame_Mode; }
	void	Set_MiniGameMode() { m_bMiniGame_Mode == true; }

	_bool	Get_MiniGameReadyCheck(_uint iNumber) { return m_bMiniGame_ReadyCheck[iNumber]; }
	void	Set_MiniGameReadyCheck(_uint iNumber, _bool _TrueOrFalse) {	m_bMiniGame_ReadyCheck[iNumber] = _TrueOrFalse;  }

	_bool	Get_MiniGameClearCheck(_uint iNumber) { return m_bMiniGame_ClearCheck[iNumber]; }
	void	Set_MiniGameClearCheck(_uint iNumber, _bool _TrueOrFalse) { m_bMiniGame_ClearCheck[iNumber] = _TrueOrFalse; }


private:
	_bool m_bMiniGame_Mode;
	_bool m_bMiniGame_ReadyCheck[3] = { true, false, false }; //앞번호가 클리어되면 레디 True
	_bool m_bMiniGame_ClearCheck[3] = { false, false, false }; //클리어 여부 체크

	CMainGame_Arrow* pGame_Arrow = nullptr;


private:
	virtual		void	Free();
};

END