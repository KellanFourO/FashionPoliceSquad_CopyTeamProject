#pragma once

#include "GameObject.h"
#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCollider;

END

class CPlayerBullet : public CGameObject
{
public:
	explicit CPlayerBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerBullet(const CPlayerBullet& rhs);
	virtual ~CPlayerBullet();

public:
	virtual	HRESULT	Ready_GameObject(_vec3 _StartPos, _int iColorIndex);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	HRESULT			Add_Component();
	void			Shoot(_vec3 _StartPos, int iColorIndex);
protected:
	CRcTex* m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	//CTransform*		m_pTransformCom = nullptr; // �̵� ������Ʈ
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

protected:
	_vec3			m_vDir, m_vPos;

	_float			m_fLiveTime;
	_bool			m_bShooting = false;

	_tchar*			m_pColorTag;
	_int			m_iColorIndex;

	_vec3			m_vStartPos;

	D3DXCOLOR		m_FilterColor; // ���� ����
	_float			m_fBulletMoveRight, m_fBulletMoveDown, m_fBulletSpeed; // �������� ������ǥ�� �̵���Ű�� ���� ����

protected:
	virtual void Free()	override;

};

