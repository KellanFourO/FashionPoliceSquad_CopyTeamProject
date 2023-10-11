#pragma once
#include "GameObject.h"
#include "MyUI.h"

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;


class CCrossHair : public Engine::CGameObject
{

private:
	explicit CCrossHair(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCrossHair(const CCrossHair& rhs);
	virtual ~CCrossHair();

public:
	virtual void			Render_GameObject() override;
	virtual HRESULT			Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;

public:
	_int					Get_CrossHairIndex() { return m_iCrossHairIndex; }

	void					Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void					Set_CrossHair(_int _CrossHairIndex) { m_iCrossHairIndex = _CrossHairIndex; }

private:
	HRESULT					Add_Component();
	void					Sprite_Function(const _float& fTimeDelta);
	void					Mouse_Chase();

private:
	CUITex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_int					m_iCrossHairIndex = 0;

	_matrix					m_matProj, m_matView;

	CGameObject*			m_pTarget;

public:
	static CCrossHair*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free() override;
};

END

