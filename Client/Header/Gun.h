#pragma once
#include "Base.h"
#include "GameObject.h"

class CGunState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CGun : public Engine::CGameObject
{
protected:
	explicit CGun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGun(const CGun& rhs);
	virtual ~CGun();

public:
	virtual	HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	LateUpdate_GameObject();
	virtual void	Render_GameObject() {};

public:
	void Gun_Fire() {};

protected:
	_float m_fBulletCoolTime = 0.f; // 총알 쿨타임

	_bool			m_bFix = false;
	_bool			m_bCheck = false;

	_bool  m_bCharged = false;//충전 확인
	_float m_fFireTime = 0.f;

	CGunState* GunState = nullptr;	// 총 상태
	CGameObject* m_pPlayer = nullptr;

public:
	_float			m_fGunMoveRight = 0.f, m_fGunMoveDown = 0.f; // 총 위치 이동
	_float			m_fBulletTime	= 0.f;  //총알 타이머
	_float			m_fTimeReload	= 0.f;
	_float			m_fBulletCount	= 0.f;	//현재 탄창에 총알
	_float			m_fMaxBullet	= 0.f;  //최대 총알

	_bool			m_bFire			= false;	// 발포 확인
	_bool			m_bIsBullet		= true;		// 총알 장전 확인
	_bool			m_bReload		= false;	// 재장전 상태 확인
	_bool			m_bGun_Select	= false;	// 총 변경 상태 확인
	_bool			m_bReady		= false;	//

	_vec3			m_vGunScale;
	_vec3			m_vShotPos;

	GunID			GUN_ID			= GunID::GUN_ID_END;

protected:
	virtual void Free();
};

