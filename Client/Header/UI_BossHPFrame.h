#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Stage1Boss.h"


// TODO ���¿� �ۼ�

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CBossHPFrame : public Engine::CGameObject
{

private:
	explicit					 CBossHPFrame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CBossHPFrame(const CBossHPFrame& rhs);
	virtual						~CBossHPFrame();

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
	_bool						m_bLateInit = true;

	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CStage1Boss*				m_pBoss = nullptr;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;

	wstring						m_wstrTitle;
	wstring						m_wstrObjective;

	CPlayer*					m_pPlayer = nullptr;
	UIDATA						m_tInfo;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CBossHPFrame*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


