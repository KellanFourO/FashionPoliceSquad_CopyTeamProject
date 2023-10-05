#pragma once

#include "Camera.h"

class CUICamera : public Engine::CCamera
{
private:
	explicit CUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void		Check_KeyInput(const _float& fTimeDelta);
	void		Mouse_Move();

	void		Mouse_Fix();



private:

	_vec3   vLook = { 0.f,0.f,1.f };
	_float		m_fSpeed = 10.f;
	_bool		m_bFix = false;
	_bool		m_bCheck = false;

public:
	static CUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void Free();



};

