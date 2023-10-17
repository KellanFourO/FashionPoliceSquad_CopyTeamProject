#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_Player :   public CGameObject
{
private:
	explicit			 CMini_Player(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Player(const CMini_Player& rhs);
	virtual				~CMini_Player();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

private:
	HRESULT				Add_Component();

public:
	MINIGAME_Player_Dir Get_Player_Dir() { return m_ePlayer_Dir; }
	void				Set_Move();
	void				Set_Dir();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 1;

	_vec3					m_PlayerPos;
	_vec3					m_PlayerScale;
	_float					fSpeed = 1.f;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;

	MINIGAME_Player_Dir		m_ePlayer_Dir = MINIGAME_Player_Dir::RIGHT;

public:
	static CMini_Player* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		 Free();


};

