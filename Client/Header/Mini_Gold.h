#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_Gold :   public CGameObject
{
private:
	explicit			 CMini_Gold(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Gold(const CMini_Gold& rhs);
	virtual				~CMini_Gold();

public:
	enum class GoldState { Live, Dead };


	virtual HRESULT		Ready_GameObject(_float pPos_X);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

	_vec3				Get_Pos() { return m_GoldPos; }
	_float				Get_Size() { return m_GoldSize; }

	void				Set_Dead() { m_GoldState = GoldState::Dead; }
	GoldState			Get_Dead() { return m_GoldState; }

private:
	HRESULT				Add_Component();
	void				Set_Move();
	void				Set_Animation();

public:


private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex = 0;
	_int					m_iTextureIndexEnd = 5;

	_int					m_iTexTick = 0;
	_int					m_iTexTickMAX = 5;

	_float					fSpeed = 3.f;

	_vec3					m_GoldPos;
	_float					m_GoldSize;

	GoldState				m_GoldState;
	_int					m_DeadCount;


private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;


public:
	static CMini_Gold* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float pPos_X);
private:
	virtual void		Free();


};

