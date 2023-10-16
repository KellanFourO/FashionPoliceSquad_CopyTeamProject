#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Stage1Boss.h"

// TODO Á¤½Â¿ë ÀÛ¼º

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CBossHPValue : public Engine::CGameObject
{

private:
	explicit					 CBossHPValue(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CBossHPValue(const CBossHPValue& rhs);
	virtual						~CBossHPValue();

public:
	virtual HRESULT				Ready_GameObject();
	virtual void				Render_GameObject() override;
	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
	virtual void				LateUpdate_GameObject() override;


private:
	HRESULT						Add_Component();


private:

	CUITex*						m_pBufferCom	= nullptr;
	CTexture*					m_pTextureCom	= nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CStage1Boss*				m_pBoss			=	nullptr;

	_bool						m_bLateInit = true;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;
	_float						m_fX, m_fY, m_fSizeX, m_fSizeY;

	UIDATA						m_tInfo;

public:
	static CBossHPValue*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


