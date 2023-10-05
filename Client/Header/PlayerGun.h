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
	_bool				m_bCharged			= false; // ���� Ȯ��
	_bool				m_bFire				= false; // ���콺�� ������ �ʾ����� ����
	_bool				m_bLateInit			= false;

	_float				m_fFireSpeed		= 0.f;   // �߻�ӵ�
	_float				m_fReloadSpeed		= 0.f;	 // �����ӵ�
	_float				m_fRebound			= 0.f;	 // �ѱ� �ݵ�

	_float				m_fGunMoveRight = 0.f;	 // ���� x�� ��ġ
	_float				m_fGunMoveDown = 0.f;	 // ���� y�� ��ġ

	_int				m_iCurrentBullet = 0.f;	 // ���� �Ѿ�
	_int				m_iMaxBullet = 0.f;		 // �ִ� �Ѿ�
	_int				m_iReloadBullet = 0.f;   // ���� �Ѿ�


	_vec3				m_vScale;	 // ������
	_vec3				m_vShotPos;	 // �ѱ� ��ġ
	_vec3				m_vShotDir;  // �߻��� ����

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

