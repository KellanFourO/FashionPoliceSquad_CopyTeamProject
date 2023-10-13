#pragma once
#include "GameObject.h"
#include "Monster.h"
#include "NewFPSCamera.h"
// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CMonsterHPBar : public Engine::CGameObject
{

private:
	explicit					 CMonsterHPBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CMonsterHPBar(const CMonsterHPBar& rhs);
	virtual						~CMonsterHPBar();

public:
	virtual HRESULT				Ready_GameObject();
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;


private:
	HRESULT						Add_Component();


private:

	CUITex*						m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	_bool						m_bLateInit = true;

	CTransform*					m_pTransformCom = nullptr;
	CNewFPSCamera*				m_pCamera = nullptr;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;

	CMonster*					m_pMonster = nullptr;
	CPlayer*					m_pPlayer = nullptr;
	UIDATA						m_tInfo;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CMonsterHPBar*			Create(LPDIRECT3DDEVICE9 pGraphicDev, CMonster* _pMonster);

private:
	virtual void				Free() override;
};

END


