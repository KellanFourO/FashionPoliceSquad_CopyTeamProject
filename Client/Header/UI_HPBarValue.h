#pragma once
#include "GameObject.h"
#include "Player.h"


// TODO ���¿� �ۼ�

BEGIN(Engine)

class CUITex;
class CTexture;
class CTransform;

class CHPBarValue : public Engine::CGameObject
{

private:
	explicit					 CHPBarValue(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit					 CHPBarValue(const CHPBarValue& rhs);
	virtual						~CHPBarValue();

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


	CTransform*					m_pTransformCom = nullptr;

	_matrix						m_matProj, m_matView;

	_vec3						m_vPos, m_vScale;

	CPlayer*					m_pPlayer = nullptr;
	UIDATA						m_tInfo;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CHPBarValue*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free() override;
};

END


