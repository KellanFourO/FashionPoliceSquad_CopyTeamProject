#pragma once
#include "GameObject.h"



BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMainGame_Quiz :  public CGameObject
{
private:
	explicit	CMainGame_Quiz(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CMainGame_Quiz(const CMainGame_Quiz& rhs);
	virtual		~CMainGame_Quiz();

public:
	enum MINIGAME_ARROW_STATEICON { STATEICON_NOP = 0, STATEICON_NORMAL, STATEICON_GOOD, STATEICON_PERPECT };
	enum class QuizGameState { ING, LOSE, CLEAR, State_END };

	virtual HRESULT			Ready_GameObject();
	virtual void			Render_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;

private:
	HRESULT					Add_Component();
	void					KeyInput();

	HRESULT					GameState_Update();

private:
	QuizGameState			m_eGameState = QuizGameState::State_END;

	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_matrix					m_matProj, m_matView;

	//CMini_Cursor* m_pCursor = nullptr;
	
	_bool					m_ClearCheck = false;

	//Å° ¾ÃÈû ¹æÁö¿ë
	bool m_bUpPressed = false;

public:
	static CMainGame_Quiz* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free() override;


};

END