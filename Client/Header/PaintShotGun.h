#pragma once

#include "Base.h"

#include "PlayerGun.h"
#include "MuzzleFlash.h"
#include "Lazer.h"

class CPlayerGunState;

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CPaintShotGun : public CPlayerGun
{
private:
	explicit		CPaintShotGun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit		CPaintShotGun(const CPaintShotGun& rhs);
	virtual			~CPaintShotGun();

	enum SHOTGUNSTATE { IDLE, WALK, JUMP, READY, SHOT, RELOAD, LAZER, ENTER, SHOTGUNSTATE_END};

public:
	virtual	HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;
	virtual void		ReadyState();

public:
	CPlayerGunState*	Get_State(_int _index) { return m_pStateArray[_index]; }
	_int				Get_BulletColr() { return m_iBulletColor;}
	_int				Get_TextureIndex() { return m_iTextureIndex;}
	CMuzzleFlash*		Get_Flash() { return m_pFlash;}
	CLazer*				Get_Lazer()	{ return m_pLazer;}
	_bool				Get_BoolLazer() { return m_bLazer;}


	void				Set_BoolLazer(_bool _bLazer) { m_bLazer = _bLazer;}
	void				Set_Flash(CMuzzleFlash* _pFlash) { m_pFlash = _pFlash;}
	void				Set_Index(_int _iIndex) { m_iTextureIndex = _iIndex; }

	void				Add_BulletColor()
	{
	++m_iBulletColor;
	if(m_iBulletColor > 8)
		m_iBulletColor = 0;
	}

private:
	HRESULT				Add_Component();

private:
	CRcTex*				m_pBufferCom		= nullptr; // 텍스처를 그리기위한 버퍼 컴포넌트
	CTexture*			m_pTextureCom		= nullptr; // 텍스쳐 컴포넌트

	_bool				m_bLazer = false;
	_int				m_iBulletColor		= 0;
	_int				m_iTextureIndex = 0;

	CMuzzleFlash*		m_pFlash = nullptr; // 총 이펙트
	CLazer*				m_pLazer = nullptr;
	CPlayerGunState*	m_pStateArray[SHOTGUNSTATE_END];

public:
	static CPaintShotGun* Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer);

private:
	virtual void	Free();
};

