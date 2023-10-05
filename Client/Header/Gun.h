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
	_float m_fBulletCoolTime = 0.f; // �Ѿ� ��Ÿ��

	_bool			m_bFix = false;
	_bool			m_bCheck = false;

	_bool  m_bCharged = false;//���� Ȯ��
	_float m_fFireTime = 0.f;

	CGunState* GunState = nullptr;	// �� ����
	CGameObject* m_pPlayer = nullptr;

public:
	_float			m_fGunMoveRight = 0.f, m_fGunMoveDown = 0.f; // �� ��ġ �̵�
	_float			m_fBulletTime	= 0.f;  //�Ѿ� Ÿ�̸�
	_float			m_fTimeReload	= 0.f;
	_float			m_fBulletCount	= 0.f;	//���� źâ�� �Ѿ�
	_float			m_fMaxBullet	= 0.f;  //�ִ� �Ѿ�

	_bool			m_bFire			= false;	// ���� Ȯ��
	_bool			m_bIsBullet		= true;		// �Ѿ� ���� Ȯ��
	_bool			m_bReload		= false;	// ������ ���� Ȯ��
	_bool			m_bGun_Select	= false;	// �� ���� ���� Ȯ��
	_bool			m_bReady		= false;	//

	_vec3			m_vGunScale;
	_vec3			m_vShotPos;

	GunID			GUN_ID			= GunID::GUN_ID_END;

protected:
	virtual void Free();
};

