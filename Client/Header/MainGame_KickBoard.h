#pragma once
#include "GameObject.h"

#include "Mini_Player.h"
#include "Mini_Enemy.h"
#include "Mini_Gold.h"
#include "Mini_Timer.h"
#include "Mini_Score.h"


BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMainGame_KickBoard :  public CGameObject
{
public:
	explicit	CMainGame_KickBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CMainGame_KickBoard(const CMainGame_KickBoard& rhs);
	virtual		~CMainGame_KickBoard();

public:
	enum class KickBoard_GameState { ING, LOSE, CLEAR, State_END };

	virtual HRESULT			Ready_GameObject();
	virtual void			Render_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;

private:
	HRESULT					Add_Component();
	HRESULT					GameState_Update();
	void					KeyInput();
	void					TimeCheck();
	void					CoinCount();

	void					Collisoin_Check();

private:

	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_matrix					m_matProj, m_matView;
	_int					m_Timer_Score_Num = 3;

	_int					m_iMyCoinCount = 0;

	CMini_Player*			m_pPlayer = nullptr;
	vector<CMini_Enemy*>	m_vecEnemy;
	vector<CMini_Gold*>		m_vecGold;

	CMini_Timer*			m_pTimer[3];
	CMini_Score*			m_pScore[3];

	M_RECT					m_defRect;
	_float					RectSizeX;
	_float					STDPointX;

	KickBoard_GameState		m_eGameState = KickBoard_GameState::State_END;

	_int					m_TimeFrame = 60;
	_int					m_RealTime = 30;

	_int					m_EnemyCreatCount = 0;
	_int					m_GoldCreatCount = 0;
	
	_bool					m_ClearCheck = false;



	//Å° ¾ÃÈû ¹æÁö¿ë
	bool m_bMousePressed = false;

public:
	static CMainGame_KickBoard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free() override;

};

END