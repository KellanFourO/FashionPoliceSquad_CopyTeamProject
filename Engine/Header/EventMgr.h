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

	void	OnLevelUp(LPDIRECT3DDEVICE9 pGraphicDev,SCENETAG eSceneTag); // ī�� ���� �� ��Ÿ �̺�Ʈ
	void	OnDialog(LPDIRECT3DDEVICE9 pGraphicDev, SCENETAG eSceneTag); // ���̾�α� ����
	void	OnPause(LAYERTAG eLayerTag, OBJECTTAG eObjectTag);

private:
	virtual		void	Free();
};

END