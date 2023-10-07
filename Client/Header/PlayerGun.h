#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Bullet.h"
//#include "Engine_Define.h"

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
	CTransform*		Get_HostTransform() { return m_pHostTransformCom; }
	CTexture*		Get_Texture() { return m_pTextureCom; }
	_float			Get_GunMoveDown() { return m_fGunMoveDown; }
	_float			Get_GunMoveRight() { return m_fGunMoveRight; }
	_vec3			Get_Scale()	{ return m_vScale; }
	_bool			Get_Ready() { return m_bReady; }
	_bool			Get_Fire() { return m_bFire;}
	Gun_INFO*		Get_GunInfo() { return &m_tGunInfo; }

	void			Set_Ready(_bool _bReady) { m_bReady = _bReady; }
	void			Set_Fire (_bool _bFire) { m_bFire = _bFire; }
	void			Set_Scale(_vec3 _vScale) { m_vScale = _vScale;}
	void			Set_Host(CGameObject* _pHost) { m_pHost = _pHost;}
	void			Reset_GunMoveDown() { m_fGunMoveDown = 1.f;}
	void			Reset_GunMoveRight() { m_fGunMoveRight = 3.f; }

	void			Set_GunMoveDown(_float _fMoveDown) { m_fGunMoveDown = _fMoveDown;}
	void			Set_GunMoveRight(_float _fMoveRight) { m_fGunMoveRight = _fMoveRight;}
	void			Add_GunMoveDown(_float _fMoveDown) { m_fGunMoveDown += _fMoveDown; }
	void			Add_GunMoveRight(_float _fMoveRight) { m_fGunMoveRight += _fMoveRight; }
	void			Reduce_GunMoveDown(_float _fMoveDown) { m_fGunMoveDown -= _fMoveDown; }
	void			Reduce_GunMoveRight(_float _fMoveRight) { m_fGunMoveRight -= _fMoveRight; }

	void			Mul_GunScale(_float _fMulScale) { m_vScale *= _fMulScale; }

public:
	void			Fire();
	void			Reload(_int _ColorIndex, _int _iRandomIndex); //! ColorIndex 매개변수는 샷건전용, RandomIndex는 라이플 전용
	void			Rebound();
	void			StateMachine(const _float& fTimeDelta);
	void			HostMove(const _float& fTimeDelta);
	void			MouseInput();

protected:
	_bool				m_bCharged			= false; // 충전 확인
	_bool				m_bReady			= true; // 연사속도제어
	_bool				m_bFire				= false; // 발사 여부
	_bool				m_bLateInit			= true;


	_float				m_fRebound			= 0.f;	 // 총기 반동
	_float				m_fGunMoveRight = 0.f;	 // 고정 x축 위치
	_float				m_fGunMoveDown = 0.f;	 // 고정 y축 위치

	_vec3				m_vScale;	 // 사이즈
	_vec3				m_vShotPos;	 // 총구 위치
	_vec3				m_vShotDir;  // 발사할 방향

	CPlayerGunState*	m_pGunState = nullptr; // 총 상태
	vector<CBullet*>	m_vecBullet; // 총알 관리하는 벡터

	CGameObject*		m_pHost	= nullptr;
	BULLETTYPE			m_eBulletType = BULLETTYPE::BULLETTYPE_END;

	Gun_INFO			m_tGunInfo; // UI에서 총알수를 얻기위한 구조체
protected:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pHostTransformCom = nullptr;


protected:
	virtual void Free();
};

