#include "Quat_Camera.h"
#include "Export_System.h"

CQuat_Camera::CQuat_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	//쿼터니언은 항상 단위 쿼터니언으로 초기화해서 사용
	D3DXQuaternionIdentity(&m_qRotation);
}

CQuat_Camera::~CQuat_Camera()
{
}

HRESULT CQuat_Camera::Ready_GameObject()
{
	// 뷰 행렬 생성을 삭제 : 뷰 행렬 대신 쿼터니언 행렬로 진행!
	// D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp); 
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	// 투영 행렬 생성
	Make_PerspectiveMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CQuat_Camera::Update_GameObject(const _float& fTimeDelta)
{
// 	// 순정 방식
// 	// 쿼터니언 연산 순서 주의, 각도는 다 라디안 단위로.
// 	static float fYaw = 0.0f;
// 	static float fPitch = 0.0f;
// 
// 	// 마우스 이동량
// 	_long dwMouseMoveX = Engine::Get_DIMouseMove(DIMS_X);
// 	_long dwMouseMoveY = Engine::Get_DIMouseMove(DIMS_Y);
// 
// 	// 회전 속도 조절을 위한 상수
// 	const float fRotationSpeed = 0.15f;
// 
// 	// Yaw 회전 (마우스 X 이동에 따라)
// 	fYaw += D3DXToRadian(static_cast<float>(-dwMouseMoveX)) * fRotationSpeed;
// 
// 	// Pitch 회전 (마우스 Y 이동에 따라)
// 	fPitch += D3DXToRadian(static_cast<float>(-dwMouseMoveY)) * fRotationSpeed;
// 
// 	// Pitch 각도를 제한 (상하로 회전 시 너무 뒤집히지 않도록)
// 	const float fPitchLimit = D3DXToRadian(89.0f); // 최대 89도까지 허용
// 	fPitch = max(-fPitchLimit, min(fPitchLimit, fPitch));
// 
// 	// Yaw 각도를 제한 (회전 시 너무 뒤집히지 않도록)
// 	const float fYawLimit = D3DXToRadian(89.0f);
// 	fYaw = max(-fYawLimit, min(fYawLimit, fYaw));
// 
// 	// Yaw Y축, Pitch X축, Roll Z축
// 	D3DXQUATERNION qRotation;
// 	D3DXQuaternionRotationYawPitchRoll(&qRotation, fYaw, fPitch, 0.0f);
// 
// 	// 현재 회전 쿼터니언에 적용
// 	m_qRotation = qRotation;
// 
// 	// 뷰 행렬 생성
// 	Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
// 	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
// 
// 	return 0;



	//보간 방식
	//쿼터니언 연산 순서 주의, 각도는 다 라디안 단위로.
	static float fYaw = 0.0f;
	static float fPitch = 0.0f;

	// 뷰 행렬 생성
	Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	// 마우스 이동량
	_long dwMouseMoveX = Engine::Get_DIMouseMove(DIMS_X);
	_long dwMouseMoveY = Engine::Get_DIMouseMove(DIMS_Y);

	// 회전 속도 조절을 위한 상수
	const float fRotationSpeedX = 0.05f;
	const float fRotationSpeedY = 0.05f;

	// 현재 회전 쿼터니언과 목표 회전 쿼터니언
	static D3DXQUATERNION qCurrentRotation = m_qRotation;
	D3DXQUATERNION qTargetRotation;

	// Yaw 회전 
	D3DXQuaternionRotationAxis(&qTargetRotation, &m_vUp, D3DXToRadian(static_cast<float>(-dwMouseMoveX)) * fRotationSpeedX);
	D3DXQuaternionMultiply(&qCurrentRotation, &qCurrentRotation, &qTargetRotation);

	// Pitch 회전
	D3DXQuaternionRotationAxis(&qTargetRotation, &m_vRight, D3DXToRadian(static_cast<float>(dwMouseMoveY)) * fRotationSpeedY);
	D3DXQuaternionMultiply(&qCurrentRotation, &qCurrentRotation, &qTargetRotation);


	// 쿼터니언이면 필요가 없는 부분이긴 한데, 혹시 몰라 만들어두고 필요시 해제해 사용
	// Pitch 각도를 제한 (상하로 회전 시 너무 뒤집히지 않도록)
	const float fPitchLimit = D3DXToRadian(89.0f); // 최대 89도까지 허용
	fPitch = max(-fPitchLimit, min(fPitchLimit, fPitch));

	// Yaw 각도를 제한 (회전 시 너무 뒤집히지 않도록)
	const float fYawLimit = D3DXToRadian(89.0f);
	fYaw = max(-fYawLimit, min(fYawLimit, fYaw));


	 // 현재 회전 쿼터니언을 보간하여 목표 쿼터니언으로 부드럽게 회전
	const float fInterpolationFactor = 1.f; // 보간 비율
	D3DXQuaternionSlerp(&m_qRotation, &m_qRotation, &qCurrentRotation, fInterpolationFactor);

	// Yaw Y축, Pitch X축, Roll Z축
	//D3DXQuaternionRotationYawPitchRoll(&m_qRotation, fYaw, fPitch, 0.0f);

	// 뷰 행렬 생성
	//Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//생성만 위에서 먼저 해보자.
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CQuat_Camera::LateUpdate_GameObject()
{

}

void CQuat_Camera::Make_ViewMatrix(D3DXMATRIX* pOut, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp)
{
	// 쿼터니언을 행렬로 변환
	D3DXMATRIX matRotation;
	D3DXMatrixIdentity(pOut); // 뷰 행렬 초기화

	 // 쿼터니언을 회전 행렬로 변환
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
	// 카메라 위치
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, -pEye->x, -pEye->y, -pEye->z);

	//D3DXVec3Normalize(&m_vRight, &D3DXVECTOR3(matRotation._11, matRotation._21, matRotation._31));

	// 뷰 행렬을 순서대로 곱하여 최종 뷰 행렬 만들기
	*pOut = matRotation * matTranslation;
}

void CQuat_Camera::Make_PerspectiveMatrix(D3DXMATRIX* pOut, float fFovy, float fAspect, float fNear, float fFar)
{
	// fFOVY : 시야각.
	// Aspect : 종횡비 = 너비 / 높이 = 가로 세로 비율
	// Near : 가까운 평면까지 거리 ( 근평면 )
	// Far : 먼 평면까지 거리 ( 원평면 )
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
