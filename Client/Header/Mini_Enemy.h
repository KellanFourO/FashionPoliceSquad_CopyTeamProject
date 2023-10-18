#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CUITex;
class CTransform;

END

class CMini_Enemy :   public CGameObject
{
private:
	explicit			 CMini_Enemy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			 CMini_Enemy(const CMini_Enemy& rhs);
	virtual				~CMini_Enemy();

public:
	enum class EnemyState { Live, Dead };

	virtual HRESULT		Ready_GameObject(_float pPos_X, int TexNum);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

	_vec3				Get_Pos() { return m_ENEMYPos; }
	_float				Get_Size() { return m_ENEMYSize; }


private:
	HRESULT				Add_Component();
	void				Set_Texture(int TexNum) { m_iTextureIndex = TexNum; }

public:
	EnemyState			Get_Dead() { return m_EnemyState; }

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iTextureIndex;

	_vec3					m_ENEMYPos;
	_vec3					m_ENEMYScale;
	_float					m_ENEMYSize;

	EnemyState				m_EnemyState;
	_int					m_DeadCount;

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_matrix					m_ViewMatrix, m_ProjMatrix;

public:
	static CMini_Enemy* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float pPos_X, int TexNum);
private:
	virtual void		 Free();


};

