#pragma once

#include "Base.h"
#include "Bullet.h"

BEGIN(Engine)

class CSYTex;
class CTexture;
class CTransform;
class CRigidBody;

END

class CBrifCase : public CBullet
{
protected:
	explicit CBrifCase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBrifCase(CBrifCase& rhs);
	virtual ~CBrifCase();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;
public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);
public:
	HRESULT Add_Component();
	void	Shot(_vec3 _StartPos);
protected:
	CSYTex*		m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTexture*	m_pTextureCom = nullptr; // �ؽ��� ������Ʈ
	CRigidBody* m_pRigidBody = nullptr; // ��ü ������Ʈ

protected:
	_vec3  m_vDir, m_vPos;

	_bool m_bShooting;

	_float		m_fFrame;
	_float		m_fAnimateTime;
	_float		m_fBulletMoveRight, m_fBulletMoveDown, m_fBulletSpeed; // �������� ������ǥ�� �̵���Ű�� ���� ����
	_float      m_fSpeed_Vertical = 0.f;

	//CTransform*	m_pPlayerTransform = nullptr;
	CTransform*	m_pHostTransform = nullptr;
public:
	static CBrifCase* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pHostTransform, CTransform* pPlayerTransform);
protected:
	virtual void Free();
};

