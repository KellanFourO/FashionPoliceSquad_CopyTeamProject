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
	enum MINIGAME_ARROW_STATEICON { STATEICON_NOP = 0, STATEICON_NORMAL, STATEICON_GOOD, STATEICON_PERPECT };
	enum class ArrowGameState { ING, LOSE, CLEAR, State_END };

	virtual HRESULT			Ready_GameObject();
	virtual void			Render_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;

private:
	HRESULT					Add_Component();
	void					KeyInput();
	
	HRESULT					State_Icon_Update();
	HRESULT					GameState_Update();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_matrix					m_matProj, m_matView;

	_bool					m_NOP_Mode = false; //Æ²·ÈÀ½!
	_int					m_iCountNop = 0;

	_int					m_ArrowCount = 30;

	//CMini_Arrow*			m_pArrow = nullptr;
	CMini_StateIcon*		m_pStateIcon = nullptr;
	CMini_Cursor*			m_pCursor = nullptr;
	CMini_TimeBar*			m_pTimeBar = nullptr;
	CMini_TimeBar*			m_pTimeBar2 = nullptr;

	vector<CMini_Arrow*>	m_pVecArrow;
	vector<CMini_Arrow*>	m_pCopyVector;

	MINIGAME_ARROW_TYPE		m_myKeyType = MINIGAME_ARROW_TYPE::MINIGAME_ARROW_END;
	ArrowGameState			m_eGameState = ArrowGameState::State_END;

	_bool					m_ClearCheck = false;


	//Å° ¾ÃÈû ¹æÁö¿ë
	bool m_bUpPressed = false;
	bool m_bDownPressed = false;
	bool m_bLeftPressed = false;
	bool m_bRightPressed = false;

public:
	static CMainGame_Arrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free() override;


};

END