#pragma once
#include "GameObject.h"
#include "NewFPSCamera.h"

class CBelt;
// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CRopeUI : public Engine::CGameObject
{

private:
	explicit					 CRopeUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CRopeUI(const CRopeUI& rhs);
	virtual						~CRopeUI();

public:
	virtual HRESULT				Ready_GameObject();
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;

	void						Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget;}

private:
	HRESULT						Add_Component();


private:

	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	_bool						m_bLateInit = true;
	_bool						m_bSearch = false;

	CTransform*					m_pTransformCom = nullptr;
	CNewFPSCamera*				m_pCamera = nullptr;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;

	CGameObject*				m_pTarget = nullptr;
	CPlayer*					m_pPlayer = nullptr;
	CBelt*						m_pBelt = nullptr;
	UIDATA						m_tInfo;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CRopeUI*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


