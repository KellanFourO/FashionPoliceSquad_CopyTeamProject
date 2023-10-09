#pragma once

#include "Base.h"
#include "Bullet.h"
class CPaintShotGun;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRigidBody;

END


class CLazer : public CBullet
{
private:
	explicit CLazer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLazer(CLazer& rhs);
	virtual ~CLazer();

public:
	virtual	HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	HRESULT			Add_Component();
	void			Color_Select();
	void			Set_Fire(_bool _bFire) { m_bFire = _bFire;}

	void			StartPosition();
	void			FirePosition(const _float& fTimeDelta);


private:
	void			Mouse_Input();

private:
	CRcTex*		m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture*	m_pTextureCom = nullptr;
	CTransform*	m_pPlayerTransform = nullptr;


private:
	_bool			m_bFire = false;
	_bool			m_bLateInit = true;
	_vec3			m_vDir, m_vPos;

	_float			m_fGunMoveDown;
	_float			m_fGunMoveRight;
	_float			m_fRange;
	_vec3			m_vScale;

	_float			m_fLiveTime;
	_bool			m_bShooting = false;

	_tchar*			m_pColorTag;
	_int			m_iColorIndex;

	_int			m_SpinAngle2 = 0.f;
	_int			m_SpinAngle3 = 0.f;

	_vec3			m_vStartPos;

	_float			m_fAnimateTime,m_fFrame,m_fTestTime;
	_bool			CollideMob = false;

	_matrix			m_Mat_Axis;

private:
	CPaintShotGun*	m_pShotGun = nullptr;

public:
	static CLazer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free()	override;
};

