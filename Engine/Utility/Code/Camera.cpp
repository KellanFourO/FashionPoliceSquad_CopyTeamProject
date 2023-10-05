#include "Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_GameObject()
{

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov,m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	return S_OK;
}

_int CCamera::Update_GameObject(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	return 0;
}

void CCamera::LateUpdate_GameObject()
{

}

void CCamera::Make_ViewMatrix(D3DXMATRIX* pOut, const _vec3* pEye, const _vec3* pAt ,const _vec3* pUp)
{
	D3DXMatrixIdentity(pOut);

	_vec3 vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);
	
	_vec3 vRight;
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);


	memcpy(&pOut->m[0][0], &vRight, sizeof(_vec3));
	memcpy(&pOut->m[1][0], &vUp, sizeof(_vec3));
	memcpy(&pOut->m[2][0], &vLook, sizeof(_vec3));
	memcpy(&pOut->m[3][0], pEye, sizeof(_vec3));
	D3DXMatrixInverse(pOut, nullptr, pOut);
	
}

void CCamera::Make_PerspectiveMatrix(D3DXMATRIX* pOut, float fFovy, float fAspect, float fNear, float fFar)
{
	D3DXMatrixIdentity(pOut);
	
	float YSize = 1.0f / tanf(fFovy * 0.5f);
	float XSize = YSize / fAspect;
	int i = 0;

	
	pOut->m[0][0] = XSize;
	pOut->m[1][1] = YSize;
	pOut->m[2][2] = fFar / (fFar - fNear);
	pOut->m[2][3] = 1.0f;
	pOut->m[3][2] = -fNear * fFar / (fFar - fNear);
	pOut->m[3][3] = 0.0f;

}

void CCamera::Free()
{
	CGameObject::Free();
}
