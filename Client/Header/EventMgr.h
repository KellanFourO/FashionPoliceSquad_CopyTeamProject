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

	void	OnLevelUp(LPDIRECT3DDEVICE9 pGraphicDev,SCENETAG eSceneTag); // ī�� ���� �� ��Ÿ �̺�Ʈ
	void	OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag, DIALOGTAG eDialogTag); // ���̾�α� ����
	void	OnPause(_bool bPause, SCENETAG eSceneTag);

	void	OnMiniGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);
	void	OnMiniGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag);



private:
	virtual		void	Free();
};

END