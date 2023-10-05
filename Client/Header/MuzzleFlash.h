#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMuzzleFlash : public Engine::CGameObject
{
private:
	explicit CMuzzleFlash(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMuzzleFlash(const CMuzzleFlash& rhs);
	virtual ~CMuzzleFlash();

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

public:
	static CMuzzleFlash* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

