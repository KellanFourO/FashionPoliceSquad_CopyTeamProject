#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Bullet.h"


class CPlayerGunState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CPlayerGun : public Engine::CGameObject
{
protected:
	explicit CPlayerGun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerGun(const CPlayerGun& rhs);
	virtual ~CPlayerGun();

public:
	virtual	HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	LateUpdate_GameObject();
	virtual void	Render_GameObject() {};
	virtual void	Ready_State();

public:
	CRcTex*			Get_Buffer() { return m_pBufferCom; }
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CTexture*		Get_Texture() { return m_pTextureCom; }

	void			Set_Host(CGameObject* _pHost) { m_pHost = _pHost;}

protected:
	void			Fire();
	void			Reload();
	void			Rebound();
	void			StateMachine();
	void			HostMove(const _float& fTimeDelta);
	void			MouseInput();

protected:
	_bool				m_bCharged			= false; // 충전 확인
	_bool				m_bFire				= false; // 마우스가 눌리지 않았을때 제어
	_bool				m_bLateInit			= false;

	_float				m_fFireSpeed		= 0.f;   // 발사속도
	_float				m_fReloadSpeed		= 0.f;	 // 장전속도
	_float				m_fRebound			= 0.f;	 // 총기 반동

	_float				m_fGunMoveRight = 0.f;	 // 고정 x축 위치
	_float				m_fGunMoveDown = 0.f;	 // 고정 y축 위치

	_int				m_iCurrentBullet = 0.f;	 // 현재 총알
	_int				m_iMaxBullet = 0.f;		 // 최대 총알
	_int				m_iReloadBullet = 0.f;   // 장전 총알


	_vec3				m_vScale;	 // 사이즈
	_vec3				m_vShotPos;	 // 총구 위치
	_vec3				m_vShotDir;  // 발사할 방향

	vector<CBullet*>	m_vecBullet;

	CGameObject*		m_pHost	= nullptr;
	BULLETTYPE			m_eBulletType = BULLETTYPE::BULLETTYPE_END;

protected:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pHostTransformCom = nullptr;


protected:
	virtual void Free();
};

