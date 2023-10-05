#include "stdafx.h"
#include "..\Header\FPSCamera.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"


CFPSCamera::CFPSCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CFPSCamera::~CFPSCamera()
{
}

HRESULT CFPSCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

Engine::_int CFPSCamera::Update_GameObject(const _float& fTimeDelta)
{
	Check_KeyInput(fTimeDelta);
	
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
	NULL_CHECK_RETURN(pPlayerTransCom, -1);

	_vec3	vPlayerPos;

	pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

	m_vAt = vPlayerPos + vLook;
	m_vEye = vPlayerPos;



	if (false == m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
	}




	_int	iExit = CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFPSCamera::Check_KeyInput(const _float& fTimeDelta)
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

void CFPSCamera::Mouse_Move()
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

void CFPSCamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

CFPSCamera* CFPSCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{

	CFPSCamera* pInstance = new CFPSCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("FPSCamera Create Failed");
		return nullptr;
	}


	return pInstance;
}

void CFPSCamera::Free()
{
	CCamera::Free();
}
