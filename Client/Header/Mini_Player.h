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
	void				Set_Rect(M_RECT& pRect) { m_PlayerRect = pRect; }
	void				Set_Move();
	void				Set_Damage();
	void				Set_DamageCount() {	m_DamageCount = 5.f; }
	void				Set_Dir();

	_vec3				Get_Pos() { return m_PlayerPos; }
	_float				Get_Size() { return m_PlayerSize; }

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 2;
	_int					m_DamageCount;

	_vec3					m_PlayerPos;
	_vec3					m_PlayerScale;
	_float					m_PlayerSize;
	_float					fSpeed = 3.f;

	M_RECT					m_PlayerRect;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;

	MINIGAME_Player_Dir		m_ePlayer_Dir = MINIGAME_Player_Dir::RIGHT;

public:
	static CMini_Player* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		 Free();


};

