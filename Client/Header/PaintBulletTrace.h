#pragma once

#include "GameObject.h"

BEGIN(Engine)


class CEffectTex;
class CTexture;
class CTransform;
class CCalculator;
class CRigidBody;

class CPaintBulletTrace
	: public Engine::CGameObject
{
private:
	explicit CPaintBulletTrace(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPaintBulletTrace(const CPaintBulletTrace& rhs);
	virtual ~CPaintBulletTrace();

public:
	virtual void			Render_GameObject() override;
	virtual HRESULT			Ready_GameObject() override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			LateUpdate_GameObject() override;
public:
	void					Set_ColorTag(COLORTAG pColorTag) { m_pColorTag = pColorTag; }
	void					Set_ShotDir(_vec3 ShotDir) { m_vShotDir = ShotDir; }

	_bool					Get_Collision() { return m_bCollision;}

private:
	HRESULT					Add_Component();

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	CEffectTex*			m_pEffectBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;
	CRigidBody*			m_pRigidBody = nullptr;

	_float				m_fFrame= 0.f;
	_float				m_PaintBulletTraceDeadTime = 0.f;
	_bool				m_bDead = false;
	COLORTAG			m_pColorTag = COLORTAG::RED;
	_vec3				m_vShotDir;

	_bool				m_bCollision = false;

public:
	static CPaintBulletTrace* Create(LPDIRECT3DDEVICE9 pGraphicDev,COLORTAG pColorTag, _vec3 vCreatePos);


private:
	virtual void Free() override;
};

END