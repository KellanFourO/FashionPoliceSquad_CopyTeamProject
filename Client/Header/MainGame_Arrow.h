#pragma once
#include "GameObject.h"

#include "Mini_Arrow.h"
#include "Mini_Cursor.h"
#include "Mini_TimeBar.h"
#include "Mini_StateIcon.h"


BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;


class CMainGame_Arrow :  public CGameObject
{
private:
	explicit	CMainGame_Arrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CMainGame_Arrow(const CMainGame_Arrow& rhs);
	virtual		~CMainGame_Arrow();

public:
	virtual HRESULT			Ready_GameObject();
	virtual void			Render_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;

private:
	HRESULT					Add_Component();
	void					KeyInput();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_matrix					m_matProj, m_matView;

	CMini_Arrow*			m_pArrow = nullptr;
	CMini_StateIcon*		m_pStateIcon = nullptr;
	CMini_Cursor*			m_pCursor = nullptr;
	CMini_TimeBar*			m_pTimeBar = nullptr;


public:
	static CMainGame_Arrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free() override;


};

END