#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRigidBody;

END

class CRifle_Bullet : public Engine::CGameObject
{
private:
	explicit CRifle_Bullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRifle_Bullet(const CRifle_Bullet& rhs);
	virtual ~CRifle_Bullet();

public:
	virtual	HRESULT	Ready_GameObject(_vec3 _StartPos, _int iRandomIndex);
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
	void			Shoot(_vec3 _StartPos, int iColorIndex);
private:
	CRcTex* m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	//CTransform* m_pTransformCom = nullptr; // �̵� ������Ʈ
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

private:
	_vec3			m_vDir, m_vPos;

	_float			m_fLiveTime;
	_bool			m_bShooting = false;

	_tchar*			m_pColorTag;
	_int			m_iColorIndex;

	_vec3			m_vStartPos;

	D3DXCOLOR		m_FilterColor; // ���� ����
	_float			m_fBulletMoveRight, m_fBulletMoveDown, m_fBulletSpeed; // �������� ������ǥ�� �̵���Ű�� ���� ����




public:
	static CRifle_Bullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _StartPos, _int iRandomIndex);

private:
	virtual void Free()	override;
};

