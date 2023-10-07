#pragma once

#include "Base.h"
#include "GameObject.h"
#include "PlayerGun.h"

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
	CTexture* m_pTextureCom = nullptr; // �ؽ��� ������Ʈ

public:
	HRESULT Add_Component();
	void	Set_Fire(_bool _bFire) { m_bFire = true;}
	void	Mouse_Input();

private:
	CPlayerGun* m_pPlayerGun = nullptr;

	_bool  m_bFire = false;
	_bool  m_bLateInit = true;
	_float m_fGunMoveRight, m_fGunMoveDown;

	_float m_fRenderingTick = 0.f;


	CTransform*	m_pPlayerTransform = nullptr;

public:
	static CMuzzleFlash* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

