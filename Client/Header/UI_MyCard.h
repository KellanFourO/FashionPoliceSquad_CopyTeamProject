#pragma once
#include "GameObject.h"
#include "UI_MyCardTexture.h"


// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMyCard : public Engine::CGameObject
{

private:
	explicit					 CMyCard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CMyCard(const CMyCard& rhs);
	virtual						~CMyCard();

public:
	virtual HRESULT				Ready_GameObject(_float StartX);
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

public:
	_bool						Get_Picking() { return m_bPicking; }
	void						Set_Picking(_bool _bPicking) { m_bPicking = _bPicking; }
	void						Set_RealPick(_bool _bRealPick) { m_bRealPick = _bRealPick; }
	void						Set_CardType(CARD_TYPE eCardType) { m_eCardType = eCardType; }

private:
	HRESULT						Add_Component();
	void						CardAnimation(const _float& fTimeDelta);
	_bool						CardOpen(const _float& fTimeDelta);
	void						CardPicking(const _float& fTimeDelta);
/*	void						RandomCard();*/
	void						SelectTexture();


private:
	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureFrontCom = nullptr;
	CTexture*					m_pTextureBackCom = nullptr;

	CMyCardTexture*				m_pCardTexture = nullptr;

	CTransform*					m_pTransformCom = nullptr;

	_matrix						m_matProj, m_matView;

	_bool						m_bStart, m_bface, m_bPicking;
	_bool						m_bOnDialog = false;
	_bool						m_bRealPick = false;

	_float						m_fSpeed, m_fMultiply, m_fMinMoveY, m_fCenterMoveY, m_fMaxMoveY, m_fAngle, m_fTimeDelta;

	_vec3						m_vPos, m_vScale;
	RECT						m_tRect;

	CARD_DIR					m_eCardDir;
	CARD_TYPE					m_eCardType = CARD_TYPE::CARD_TYPE_END;

public:
	static CMyCard*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _float StartX, CARD_DIR Dir, CARD_TYPE eCardType);

private:
	virtual void				Free() override;
};

END


