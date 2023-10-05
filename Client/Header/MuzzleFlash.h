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
	CRcTex* m_pBufferCom = nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTransform* m_pTransformCom = nullptr; // 이동 컴포넌트
	CTexture* m_pTextureCom = nullptr; // 텍스쳐 컴포넌트

public:
	HRESULT Add_Component();
	void	Set_Gun(CGameObject* _pGun) { m_pGun = _pGun; };

private:
	CGameObject* m_pGun; // 발사 확인을 위한 타겟

	_float m_fGunMoveRight, m_fGunMoveDown;
	_float m_fBulletTime;
	_float m_fBulletColor;

public:
	static CMuzzleFlash* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

