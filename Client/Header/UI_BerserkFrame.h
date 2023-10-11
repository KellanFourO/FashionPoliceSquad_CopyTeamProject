#pragma once
#include "GameObject.h"
#include "Player.h"


// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CBerserkFrame_UI : public Engine::CGameObject
{

private:
	explicit					 CBerserkFrame_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CBerserkFrame_UI(const CBerserkFrame_UI& rhs);
	virtual						~CBerserkFrame_UI();

public:
	virtual HRESULT				Ready_GameObject();
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

public:
	void						Set_Title(const _tchar* _szTitle) { m_wstrTitle = _szTitle; }
	void						Set_Objective(const _tchar* _szObjective) { m_wstrObjective = _szObjective; }

private:
	HRESULT						Add_Component();


private:

	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;


	CTransform*					m_pTransformCom = nullptr;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;

	wstring						m_wstrTitle;
	wstring						m_wstrObjective;

	CPlayer*					m_pPlayer = nullptr;
	UIDATA						m_tInfo;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CBerserkFrame_UI*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


