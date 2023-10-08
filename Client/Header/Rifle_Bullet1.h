#pragma once

#include "Base.h"
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRigidBody;

END

class CRifle_Bullet1 : public CBullet
{
private:
	explicit CRifle_Bullet1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRifle_Bullet1(const CRifle_Bullet1& rhs);
	virtual ~CRifle_Bullet1();

public:
	virtual	HRESULT		Ready_GameObject(_vec3 _StartPos, _int iColorIndex);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;


public:
	HRESULT				Add_Component();

private:
	CRcTex*				m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTexture*			m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

private:
	_vec3			m_vDir, m_vPos;

	_float			m_fLiveTime;
	_vec3			m_vStartPos;

	D3DXCOLOR		m_FilterColor; // ���� ����
	_float			m_fBulletMoveRight, m_fBulletMoveDown, m_fBulletSpeed; // �������� ������ǥ�� �̵���Ű�� ���� ����




public:
	static CRifle_Bullet1* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _StartPos, _int iColorIndex);

private:
	virtual void Free()	override;
};

