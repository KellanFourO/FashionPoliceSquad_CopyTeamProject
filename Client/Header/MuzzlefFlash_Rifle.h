#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMuzzleFlash_Rifle : public Engine::CGameObject
{
private:
	explicit CMuzzleFlash_Rifle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMuzzleFlash_Rifle(const CMuzzleFlash_Rifle& rhs);
	virtual ~CMuzzleFlash_Rifle();

public:
	virtual	HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	CRcTex* m_pBufferCom = nullptr; // �ؽ�ó�� �׸������� ���� ������Ʈ
	CTransform* m_pTransformCom = nullptr; // �̵� ������Ʈ
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

public:
	HRESULT Add_Component();
	void	Set_Gun(CGameObject* _pGun) { m_pGun = _pGun; };

private:
	CGameObject* m_pGun; // �߻� Ȯ���� ���� Ÿ��

	_float m_fGunMoveRight, m_fGunMoveDown;
	_float m_fBulletTime;
	_float m_fBulletColor;

	_int m_RandAngle;

	_int i = 0; // ������ڵ�

public:
	static CMuzzleFlash_Rifle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

