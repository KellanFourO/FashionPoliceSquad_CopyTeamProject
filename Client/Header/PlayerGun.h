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
	void			Reload(_int _ColorIndex, _int _iRandomIndex); //! ColorIndex �Ű������� ��������, RandomIndex�� ������ ����
	void			Rebound();
	void			StateMachine(const _float& fTimeDelta);
	void			HostMove(const _float& fTimeDelta);
	void			MouseInput();

protected:
	_bool				m_bCharged			= false; // ���� Ȯ��
	_bool				m_bReady			= true; // ����ӵ�����
	_bool				m_bFire				= false; // �߻� ����
	_bool				m_bLateInit			= true;


	_float				m_fRebound			= 0.f;	 // �ѱ� �ݵ�
	_float				m_fGunMoveRight = 0.f;	 // ���� x�� ��ġ
	_float				m_fGunMoveDown = 0.f;	 // ���� y�� ��ġ

	_vec3				m_vScale;	 // ������
	_vec3				m_vShotPos;	 // �ѱ� ��ġ
	_vec3				m_vShotDir;  // �߻��� ����

	CPlayerGunState*	m_pGunState = nullptr; // �� ����
	vector<CBullet*>	m_vecBullet; // �Ѿ� �����ϴ� ����

	CGameObject*		m_pHost	= nullptr;
	BULLETTYPE			m_eBulletType = BULLETTYPE::BULLETTYPE_END;

	Gun_INFO			m_tGunInfo; // UI���� �Ѿ˼��� ������� ����ü
protected:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pHostTransformCom = nullptr;


protected:
	virtual void Free();
};

