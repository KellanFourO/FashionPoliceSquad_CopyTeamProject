#include "Quat_Camera.h"
#include "Export_System.h"

CQuat_Camera::CQuat_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	//���ʹϾ��� �׻� ���� ���ʹϾ����� �ʱ�ȭ�ؼ� ���
	D3DXQuaternionIdentity(&m_qRotation);
}

CQuat_Camera::~CQuat_Camera()
{
}

HRESULT CQuat_Camera::Ready_GameObject()
{
	// �� ��� ������ ���� : �� ��� ��� ���ʹϾ� ��ķ� ����!
	// D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp); 
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	// ���� ��� ����
	Make_PerspectiveMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CQuat_Camera::Update_GameObject(const _float& fTimeDelta)
{
// 	// ���� ���
// 	// ���ʹϾ� ���� ���� ����, ������ �� ���� ������.
// 	static float fYaw = 0.0f;
// 	static float fPitch = 0.0f;
// 
// 	// ���콺 �̵���
// 	_long dwMouseMoveX = Engine::Get_DIMouseMove(DIMS_X);
// 	_long dwMouseMoveY = Engine::Get_DIMouseMove(DIMS_Y);
// 
// 	// ȸ�� �ӵ� ������ ���� ���
// 	const float fRotationSpeed = 0.15f;
// 
// 	// Yaw ȸ�� (���콺 X �̵��� ����)
// 	fYaw += D3DXToRadian(static_cast<float>(-dwMouseMoveX)) * fRotationSpeed;
// 
// 	// Pitch ȸ�� (���콺 Y �̵��� ����)
// 	fPitch += D3DXToRadian(static_cast<float>(-dwMouseMoveY)) * fRotationSpeed;
// 
// 	// Pitch ������ ���� (���Ϸ� ȸ�� �� �ʹ� �������� �ʵ���)
// 	const float fPitchLimit = D3DXToRadian(89.0f); // �ִ� 89������ ���
// 	fPitch = max(-fPitchLimit, min(fPitchLimit, fPitch));
// 
// 	// Yaw ������ ���� (ȸ�� �� �ʹ� �������� �ʵ���)
// 	const float fYawLimit = D3DXToRadian(89.0f);
// 	fYaw = max(-fYawLimit, min(fYawLimit, fYaw));
// 
// 	// Yaw Y��, Pitch X��, Roll Z��
// 	D3DXQUATERNION qRotation;
// 	D3DXQuaternionRotationYawPitchRoll(&qRotation, fYaw, fPitch, 0.0f);
// 
// 	// ���� ȸ�� ���ʹϾ� ����
// 	m_qRotation = qRotation;
// 
// 	// �� ��� ����
// 	Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
// 	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
// 
// 	return 0;



	//���� ���
	//���ʹϾ� ���� ���� ����, ������ �� ���� ������.
	static float fYaw = 0.0f;
	static float fPitch = 0.0f;

	// �� ��� ����
	Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	// ���콺 �̵���
	_long dwMouseMoveX = Engine::Get_DIMouseMove(DIMS_X);
	_long dwMouseMoveY = Engine::Get_DIMouseMove(DIMS_Y);

	// ȸ�� �ӵ� ������ ���� ���
	const float fRotationSpeedX = 0.05f;
	const float fRotationSpeedY = 0.05f;

	// ���� ȸ�� ���ʹϾ�� ��ǥ ȸ�� ���ʹϾ�
	static D3DXQUATERNION qCurrentRotation = m_qRotation;
	D3DXQUATERNION qTargetRotation;

	// Yaw ȸ�� 
	D3DXQuaternionRotationAxis(&qTargetRotation, &m_vUp, D3DXToRadian(static_cast<float>(-dwMouseMoveX)) * fRotationSpeedX);
	D3DXQuaternionMultiply(&qCurrentRotation, &qCurrentRotation, &qTargetRotation);

	// Pitch ȸ��
	D3DXQuaternionRotationAxis(&qTargetRotation, &m_vRight, D3DXToRadian(static_cast<float>(dwMouseMoveY)) * fRotationSpeedY);
	D3DXQuaternionMultiply(&qCurrentRotation, &qCurrentRotation, &qTargetRotation);


	// ���ʹϾ��̸� �ʿ䰡 ���� �κ��̱� �ѵ�, Ȥ�� ���� �����ΰ� �ʿ�� ������ ���
	// Pitch ������ ���� (���Ϸ� ȸ�� �� �ʹ� �������� �ʵ���)
	const float fPitchLimit = D3DXToRadian(89.0f); // �ִ� 89������ ���
	fPitch = max(-fPitchLimit, min(fPitchLimit, fPitch));

	// Yaw ������ ���� (ȸ�� �� �ʹ� �������� �ʵ���)
	const float fYawLimit = D3DXToRadian(89.0f);
	fYaw = max(-fYawLimit, min(fYawLimit, fYaw));


	 // ���� ȸ�� ���ʹϾ��� �����Ͽ� ��ǥ ���ʹϾ����� �ε巴�� ȸ��
	const float fInterpolationFactor = 1.f; // ���� ����
	D3DXQuaternionSlerp(&m_qRotation, &m_qRotation, &qCurrentRotation, fInterpolationFactor);

	// Yaw Y��, Pitch X��, Roll Z��
	//D3DXQuaternionRotationYawPitchRoll(&m_qRotation, fYaw, fPitch, 0.0f);

	// �� ��� ����
	//Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//������ ������ ���� �غ���.
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CQuat_Camera::LateUpdate_GameObject()
{

}

void CQuat_Camera::Make_ViewMatrix(D3DXMATRIX* pOut, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp)
{
	// ���ʹϾ��� ��ķ� ��ȯ
	D3DXMATRIX matRotation;
	D3DXMatrixIdentity(pOut); // �� ��� �ʱ�ȭ

	 // ���ʹϾ��� ȸ�� ��ķ� ��ȯ
	D3DXMatrixRotationQuaternion(&matRotation, &m_qRotation);

	_vec3 m_vLook = *pAt - *pEye;
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	_vec3 m_vRight;
	D3DXVec3Cross(&m_vRight, pUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	_vec3 vUp;
	D3DXVec3Cross(&vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	memcpy(&pOut->m[0][0], &m_vRight, sizeof(_vec3));
	memcpy(&pOut->m[1][0], &vUp, sizeof(_vec3));
	memcpy(&pOut->m[2][0], &m_vLook, sizeof(_vec3));
	memcpy(&pOut->m[3][0], pEye, sizeof(_vec3));

	D3DXMatrixInverse(pOut, nullptr, pOut);
	// ī�޶� ��ġ
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, -pEye->x, -pEye->y, -pEye->z);

	//D3DXVec3Normalize(&m_vRight, &D3DXVECTOR3(matRotation._11, matRotation._21, matRotation._31));

	// �� ����� ������� ���Ͽ� ���� �� ��� �����
	*pOut = matRotation * matTranslation;
}

void CQuat_Camera::Make_PerspectiveMatrix(D3DXMATRIX* pOut, float fFovy, float fAspect, float fNear, float fFar)
{
	// fFOVY : �þ߰�.
	// Aspect : ��Ⱦ�� = �ʺ� / ���� = ���� ���� ����
	// Near : ����� ������ �Ÿ� ( ����� )
	// Far : �� ������ �Ÿ� ( ����� )
	D3DXMatrixIdentity(pOut);

	//float Radian = D3DXToRadian(fFovy);

	float YSize = 1.0f / tanf(fFovy * 0.5f);
	float XSize = YSize / fAspect;


	pOut->m[0][0] = XSize;
	pOut->m[1][1] = YSize;
	pOut->m[2][2] = fFar / (fFar - fNear);
	pOut->m[2][3] = 1.0f;
	pOut->m[3][2] = -fNear * fFar / (fFar - fNear);
	pOut->m[3][3] = 0.0f;

}

void CQuat_Camera::Free()
{
	CGameObject::Free();
}
