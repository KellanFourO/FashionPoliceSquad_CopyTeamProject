#pragma once
#include "Engine_Define.h"
#include "Base.h"
#include "../Client/Header/CardList.h"
#include "../Client/Header/MyDialog.h"

BEGIN(Engine)

class ENGINE_DLL CEventMgr : public CBase
{
	DECLARE_SINGLETON(CEventMgr)

private:
	explicit CEventMgr();
	virtual ~CEventMgr();

public:
	void    GetEventColider();

	void	OnLevelUp(LPDIRECT3DDEVICE9 pGraphicDev,SCENETAG eSceneTag); // 카드 생성 및 기타 이벤트
	void	OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag); // 다이얼로그 생성
	void	OnPause(LAYERTAG eLayerTag, OBJECTTAG eObjectTag);

private:
	virtual		void	Free();
};

END