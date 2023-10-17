#pragma once
#include "GameObject.h"

#include "Mini_Player.h"


BEGIN(Engine)

class UITex;
class CTexture;
class CTransform;


class CMainGame_KickBoard :    public CGameObject
{
private:
	explicit	CMainGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CMainGame_KickBoard(const CMainGame_KickBoard& rhs);
	virtual		~CMainGame_KickBoard();

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


//	CMini_Arrow*			m_pArrow = nullptr;
// 	CMini_StateIcon*		m_pStateIcon = nullptr;
// 	CMini_Cursor*			m_pCursor = nullptr;
// 
// 	vector<CMini_Arrow*>	m_pVecArrow;

	_bool					m_ClearCheck = false;


	//Å° ¾ÃÈû ¹æÁö¿ë
	bool m_bLeftPressed = false;
	bool m_bRightPressed = false;


public:
	static CMainGame_KickBoard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;


};



END;