#pragma once
#include "GameObject.h"
#include "UI_MyCard.h"


// TODO ���¿� �ۼ�

BEGIN(Engine)

class CCardList : public Engine::CGameObject
{

private:
	explicit					 CCardList(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CCardList(const CCardList& rhs);
	virtual						~CCardList();

public:
	virtual HRESULT				Ready_GameObject() override;
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

	void						KeyInput();


private:
	vector<CMyCard*>			m_vecCard;
	CARD_DIR					m_eDir;
	_bool						m_bPicking = FALSE;
	int							m_iLateTick = 0;
	DIALOGTAG					m_eDialogTag = DIALOGTAG::DIALOGTAG_END;

public:
	static CCardList*			Create(LPDIRECT3DDEVICE9 pGraphicDev, DIALOGTAG eDialogTag);

private:
	virtual void				Free() override;
};

END


