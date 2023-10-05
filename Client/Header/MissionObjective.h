#pragma once
#include "GameObject.h"



// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMissionObjective : public Engine::CGameObject
{

private:
	explicit					 CMissionObjective(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CMissionObjective(const CMissionObjective& rhs);
	virtual						~CMissionObjective();

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

public:
	static CMissionObjective*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


