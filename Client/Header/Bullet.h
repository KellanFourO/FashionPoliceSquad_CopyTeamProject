#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCollider;
END

class CBullet : public Engine::CGameObject
{

protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;


public:
	CRcTex*			Get_Buffer() { return m_pBufferCom; }
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CTexture*		Get_Texture() { return m_pTextureCom; }
	BULLETTYPE		Get_BulletType() { return m_eBulletType;}

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void			Fire(_vec3 vShotPos, _vec3 vShotDir);
	void			Destroy(const _float& fTimeDelta);

protected:
	_bool			m_bLateInit = true;
	_bool			m_bShot	= false;
	_bool			m_bDead = false;


	_float			m_fDmg			= 0.f; //! ÃÑ¾Ë µ¥¹ÌÁö
	_float			m_fAge			= 0.f;
	_float			m_fLifeTime		= 3.f;
	_float			m_fTimeDelta	= 0.f;
	_float			m_fSpeed		= 0.f;
	_vec3			m_vShotDir;
	_vec3			m_vShotPos;


	BULLETTYPE	    m_eBulletType = BULLETTYPE::BULLETTYPE_END;
	OBJECTTAG		m_eTargetTag = OBJECTTAG::OBJECT_END;

protected:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pPlayerTransformCom = nullptr;


protected:
	virtual void Free();
};

