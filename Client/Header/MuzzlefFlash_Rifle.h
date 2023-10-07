#pragma once

#include "Base.h"
#include "GameObject.h"
#include "PlayerGun.h"

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
	CRcTex* m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture* m_pTextureCom = nullptr; // 텍스쳐 컴포넌트

public:
	HRESULT Add_Component();
	void	Set_Fire(_bool _bFire) { m_bFire = true; }
	void	Mouse_Input();

private:
	CPlayerGun* m_pPlayerGun = nullptr;

	_bool	m_bFire = false;
	_bool	m_bLateInit = true;
	_float	m_fGunMoveRight, m_fGunMoveDown;

	_float	m_fRenderingTick = 0.f;
	_int	m_RandAngle;

	CTransform* m_pPlayerTransform = nullptr;

public:
	static CMuzzleFlash_Rifle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

