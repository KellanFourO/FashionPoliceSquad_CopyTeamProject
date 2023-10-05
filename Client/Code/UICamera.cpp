#include "stdafx.h"
#include "UICamera.h"

#include "Export_System.h"
#include "Export_Utility.h"

CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CUICamera::~CUICamera()
{
}

HRESULT CUICamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	_matrix matWorld;
	D3DXMatrixInverse(&matWorld, nullptr, &m_matView);
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_matView._11 = 210.f;
	m_matView._22 = 222.f;
	m_matView._33 = 1.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

Engine::_int CUICamera::Update_GameObject(const _float& fTimeDelta)
{

	//D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	//D3DXMatrixOrthoOffCenterLH(&m_matProj, 0.0f, WINCX, WINCY, 0.f, 0.f, 1.f);

	return 0;
}

void CUICamera::Check_KeyInput(const _float& fTimeDelta)
{

	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;

}

void CUICamera::Mouse_Move()
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		vLook = m_vAt - m_vEye;

		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_vec3	vUp = { 0.f, 1.f, 0.f };

		vLook = m_vAt - m_vEye;

		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_vAt = m_vEye + vLook;
	}

}

void CUICamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

CUICamera* CUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{

	CUICamera* pInstance = new CUICamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UICamera Create Failed");
		return nullptr;
	}


	return pInstance;
}

void CUICamera::Free()
{
	CCamera::Free();
}
