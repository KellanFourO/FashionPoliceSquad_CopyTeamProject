#pragma once
#include "Engine_Define.h"
#include "Base.h"
#include "UI_CardList.h"
#include "UI_MyDialog.h"

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
	void	OnPause(_bool bPause);





private:
	virtual		void	Free();
};

END