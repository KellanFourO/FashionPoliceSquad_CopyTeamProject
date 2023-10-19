#include "stdafx.h"
#include "NewFPSCamera.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

CNewFPSCamera::CNewFPSCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CNewFPSCamera::~CNewFPSCamera()
{
}

HRESULT CNewFPSCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_vUp_D = *pUp;

	// Camera Shake
	m_bCameraCheck = false;
	CAMSHAKE.m_bShaking = false; // 흔들림 상태
	CAMSHAKE.m_fShakeElipsedTime = 0.f; // 중첩시간

	CAMSHAKE.m_fAmplitude = 0.005f; // 진폭
	CAMSHAKE.m_fDuration = 0.1f;  // 흔드는 시간
	CAMSHAKE.m_fFrequency = 1.f; // 흔드는 속도
	CAMSHAKE.m_vOriginPos = _vec3(0.f, 0.f, 0.f);

	Add_Component();
	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	m_eObjectTag = OBJECTTAG::FPSCAMERA;
	m_eCamera_Mode = CAMERAID::CAM_FIRST;

	pPlayerTransCom = Management()->Get_Player()->Get_Transform();
	return S_OK;
}

Engine::_int CNewFPSCamera::Update_GameObject(const _float& fTimeDelta)
{
	Check_KeyInput(fTimeDelta);


	pPlayerTransCom->Get_Info(INFO_POS, &m_vPlayerPos);

 	m_vAt = m_vPlayerPos + vLook;
 	m_vEye = m_vPlayerPos;

	if (true == m_bFix)
	{
		Mouse_Move(fTimeDelta);
		Mouse_Fix();
	}


	switch (m_eCamera_Mode)
	{
	case CAMERAID::CAM_FIRST:
		First_Camera();
		break;
	case CAMERAID::CAM_THIRD:
		//Third_Camera();
		break;
	}

	if (CAMSHAKE.m_bShaking)
	{
		CAMSHAKE.m_fShakeElipsedTime += fTimeDelta;

		if (CAMSHAKE.m_fShakeElipsedTime < CAMSHAKE.m_fDuration)
		{
			_float X = CAMSHAKE.m_fAmplitude * cosf(CAMSHAKE.m_fShakeElipsedTime * CAMSHAKE.m_fFrequency + (((_float)rand() / (_float)RAND_MAX) * D3DX_PI));
			_float Y = CAMSHAKE.m_fAmplitude * -sinf(CAMSHAKE.m_fShakeElipsedTime * CAMSHAKE.m_fFrequency + (((_float)rand() / (_float)RAND_MAX) * D3DX_PI));
			m_vEye += _vec3(X, Y, 0);
		}
		else
		{
			m_vEye = CAMSHAKE.m_vOriginPos;
			CAMSHAKE.m_bShaking = false;
		}
	}

	_int	iExit = CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CNewFPSCamera::LateUpdate_GameObject()
{
	CCamera::LateUpdate_GameObject();
}

void CNewFPSCamera::Render_GameObject()
{
}

HRESULT CNewFPSCamera::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CNewFPSCamera::Check_KeyInput(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);


	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}

	if (Engine::Get_DIKeyState(DIK_F2) & 0x80)
	{
		Shake_Camera();
	}

	if (Engine::Get_DIKeyState(DIK_F3) & 0x80)
	{
		m_eCamera_Mode = CAMERAID::CAM_FIRST;
		m_bCameraCheck = false;
	}

	if (Engine::Get_DIKeyState(DIK_F4) & 0x80)
	{
		m_eCamera_Mode = CAMERAID::CAM_THIRD;
		m_bCameraCheck = true;
		Third_Camera();
	}

	if (false == m_bFix)
		return;

}

void CNewFPSCamera::Mouse_Move(const _float& fTimeDelta)
{
#pragma region 방법 1 - Main

// 	_matrix		matCamWorld;
// 	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
// 	_vec3		vRotation;
//
// 	m_vAt =		m_vPlayerPos + vLook;
// 	m_vEye =	m_vPlayerPos;  // 이거 없애면 3인칭 처럼 됨
//
// 	_matrix		matRot;
//
// 	m_dwMouseMove[ROT_X] = Engine::Get_DIMouseMove(DIMS_X);
//  	m_dwMouseMove[ROT_Y] = Engine::Get_DIMouseMove(DIMS_Y);
//
// 	if (m_dwMouseMove[ROT_Y])  //X축을 기준으로 상하회전
// 	{
// 		_vec3 vRight;
// 		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
//
// 		//vRotation = _vec3(D3DXToRadian(m_dwMouseMove[ROT_Y]) * m_fSpeed * fTimeDelta, 0.f, 0.f);
//
// 		vRotation = _vec3(D3DXToRadian(m_dwMouseMove[ROT_Y]), 0.f, 0.f);
// 		matRot = m_pTransformCom->Rotate(vRotation);
//
// 		vLook =		m_vAt - m_vEye;
// 		pPlayerTransCom->Set_Look(vLook);
//
// 		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
// 		D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
// 	}
//
// 	if (m_dwMouseMove[ROT_X]) //Y축을 기준으로 좌우회전
// 	{
// 		//vRotation = _vec3(0.f, D3DXToRadian(m_dwMouseMove[ROT_X] / 10.f) * m_fSpeed * fTimeDelta, 0.f);
//
// 		vRotation = _vec3(0.f, D3DXToRadian(m_dwMouseMove[ROT_X] / 10.f), 0.f);
// 		matRot = m_pTransformCom->Rotate(vRotation);
//
// 		vLook = m_vAt - m_vEye;
// 		pPlayerTransCom->Set_Look(vLook);
//
// 		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
// 	}

#pragma endregion 방법 1

#pragma region 방법 2

// 	 	_matrix		matCamWorld;
// 	 	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
//
// 	 	_quat		quatPitch, quatYaw, quatRoll;
// 	 	D3DXQuaternionIdentity(&quatPitch);
// 	 	D3DXQuaternionIdentity(&quatYaw);
// 	 	D3DXQuaternionIdentity(&quatRoll);
//
// 	 	_vec3		vLook, vRotation;
// 	 	_matrix		matRot;
//
// 	 	vLook =		m_vAt - m_vEye;
// 	 	D3DXVec3Cross(&m_vRight, &m_vUp, &vLook);
//
// 	 	m_fMaxAngleY = 150.f;
// 	 	m_fMinAngleY = 90.f;
//
// 	 	m_dwMouseMove[ROT_X] = Engine::Get_DIMouseMove(DIMS_X);
// 	 	m_dwMouseMove[ROT_Y] = Engine::Get_DIMouseMove(DIMS_Y);
//
//
// 	 	////// 0. 각도가 Min - Max 범위 밖인 경우, 예외방식 회전
// 	 	if (m_fAngleY > m_fMaxAngleY)
// 	  	{
// 	  		D3DXQuaternionRotationAxis(&quatPitch, &m_vRight, D3DXToRadian(m_fAngleY - (m_fMaxAngleY - m_fAngleY)));
// 	  		m_quaternion *= quatPitch;
// 	  		D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
// 	  	}
// 	  	if (m_fAngleY < m_fMinAngleY)
// 	  	{
// 	  		D3DXQuaternionRotationAxis(&quatPitch, &m_vRight, D3DXToRadian(m_fAngleY + (m_fAngleY - m_fMinAngleY)));
// 	  		m_quaternion *= quatPitch;
// 	  		D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
// 	  	}
//
// 	 	////// 1. 각도가 Min - Max 범위 밖인지 확인하고, 문제 없으면 임의 축 회전
// 	 	_float fNewAngle = m_fAngleY + (m_dwMouseMove[ROT_Y] * m_fSpeed);
//
// 	 	// 세로 회전 (X축 회전)
// 	 	if (fNewAngle < m_fMaxAngleY && fNewAngle > m_fMinAngleY)
// 	 	{
// 	 		if (m_dwMouseMove[ROT_X])
// 	 		{
// 	 			D3DXQuaternionRotationAxis(&quatPitch, &m_vRight, D3DXToRadian(m_dwMouseMove[ROT_Y] * m_fSpeed));
// 	 			m_quaternion *= quatPitch;
// 	 			D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
// 	 		}
// 	 	}
// 	 	// 가로 회전 (Y축 회전)
// 	 	if (m_dwMouseMove[ROT_Y])
// 	 	{
// 	 		D3DXQuaternionRotationAxis(&quatYaw, &m_vUp, D3DXToRadian(m_dwMouseMove[ROT_X] * m_fSpeed));
// 	 		m_quaternion *= quatYaw;
// 	 		D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
// 	 	}
//
// 	 		_matrix matRotation; // 회전 행렬을 만들기 위한 행렬
// 	 	D3DXMatrixRotationQuaternion(&matRotation, &m_quaternion); // 쿼터니언을 회전 행렬로 변환
//
// // 	 	// 기존 코드에서 회전 행렬을 카메라 뷰 행렬에 곱함
// // 	 	D3DXMatrixMultiply(&m_matView, &matRotation, &m_matView);
// //
// 	 	_vec3 vPos = m_vEye - pPlayerTransCom->m_vInfo[INFO_POS];
// 	 	// 결과 값을 저장할 벡터 주소, 행렬과 곱하기를 할 위치 벡터 주소, 행렬 주소;
// 	 	D3DXVec3TransformCoord(&vPos, &vPos, &matRotation);      // 위치벡터
//
// 	 	m_vEye = vPos + pPlayerTransCom->m_vInfo[INFO_POS];
//
//
// 	 	// 카메라와 플레이어의 끼인 각 구하기
// 	 	// 카메라가 플레이어를 바라보는 방향 (목표물 - 시작점)
// 	 	_vec3  vCameraToPlayer = pPlayerTransCom->m_vInfo[INFO_POS] - m_vEye;
// 	 	D3DXVec3Normalize(&vCameraToPlayer, &vCameraToPlayer);   // 정규화
//
// 	 	// 카메라가 플레이어를 바라보는 방향과 플레이어의 Up벡터를 내적해서 둘 사이의 끼인각을 알아낸다(라디안)
// 	 	m_fAngleY = acosf(D3DXVec3Dot(&vCameraToPlayer, &pPlayerTransCom->m_vInfo[INFO_UP]));
// 	 	m_fAngleY = D3DXToDegree(m_fAngleY);
//
//
// 	 	// 카메라와 플레이어의 끼인 각 구하기
// 	 	// 카메라가 플레이어를 바라보는 방향 (목표물 - 시작점)
// 	 	_vec3  vCameraToPlayerX = pPlayerTransCom->m_vInfo[INFO_POS] - m_vEye;
// 	 	D3DXVec3Normalize(&vCameraToPlayerX, &vCameraToPlayerX);   // 정규화
//
// 	 	// 카메라가 플레이어를 바라보는 방향과 플레이어의 RIGHT벡터를 내적해서 둘 사이의 끼인각을 알아낸다(라디안)
// 	 	m_fAngleX = acosf(D3DXVec3Dot(&vCameraToPlayerX, &pPlayerTransCom->m_vInfo[INFO_LOOK]));
// 	 	m_fAngleX = D3DXToDegree(m_fAngleX);

#pragma endregion 방법 2


#pragma region 방법 3

	m_vAt = m_vPlayerPos + vLook;
	m_vEye = m_vPlayerPos;

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
#pragma endregion 방법 3
}

void CNewFPSCamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}


void CNewFPSCamera::First_Camera()
{
}
void CNewFPSCamera::Third_Camera()
{
	if (m_bCameraCheck)
	{
		m_vEye = _vec3(0.f, 10.f, -10.f);
		m_vAt = _vec3(0.f, 0.f, 1.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);

		m_fFar = 1000.f;
		m_fNear = 0.1f;
		m_fAspect = (_float)WINCX / WINCY;
		m_fFov = D3DXToRadian(60.f);
	}
}

void CNewFPSCamera::Reset_ShakeForce()
{
	CAMSHAKE.m_fShakeElipsedTime = 0.f; // 중첩시간

	CAMSHAKE.m_fAmplitude = 0.f; // 진폭
	CAMSHAKE.m_fDuration = 0.f;  // 흔드는 시간
	CAMSHAKE.m_fFrequency = 0.f; // 흔드는 속도
}

void CNewFPSCamera::Shake_Camera()
{
 	CComponent* pComponent = Management()->Get_Player()->Get_Transform();

 	_vec3	vPlayerPos = static_cast<CTransform*>(pComponent)->m_vInfo[INFO_POS];

 	CAMSHAKE.m_fShakeElipsedTime = 0.f;
 	CAMSHAKE.m_vOriginPos = vPlayerPos;
 	CAMSHAKE.m_bShaking = true;
}


CNewFPSCamera* CNewFPSCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye,	const _vec3* pAt,	const _vec3* pUp,	const _float& fFov,
	const _float& fAspect,	const _float& fNear,	const _float& fFar)
{
	CNewFPSCamera* pInstance = new CNewFPSCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("FPSCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNewFPSCamera::Free()
{
	CCamera::Free();
}



/////////////////////////////////////////////////////////////////////

void CNewFPSCamera::Strafe(float units)
{
	if (units == 0)
		return;

}

void CNewFPSCamera::Fly(float units)
{
}

void CNewFPSCamera::Walk(float units)
{
}

void CNewFPSCamera::Pitch(float fAngle)
{
	_matrix T;
	D3DXMatrixRotationAxis(&T, &m_vRight_D, fAngle);

	D3DXVec3TransformCoord(&m_vUp_D, &m_vUp_D, &T);
	D3DXVec3TransformCoord(&m_vLook_D, &m_vLook_D, &T);
}

void CNewFPSCamera::Yaw(float fAngle)
{
	_matrix T;
	if (m_eCameraType == CAMERATYPE::CAM_LAND_OBJ)
		D3DXMatrixRotationY(&T, fAngle);

	if (m_eCameraType == CAMERATYPE::CAM_AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &m_vUp_D, fAngle);

	D3DXVec3TransformCoord(&m_vRight_D, &m_vRight_D, &T);
	D3DXVec3TransformCoord(&m_vLook_D, &m_vLook_D, &T);
}

void CNewFPSCamera::Roll(float fAngle)
{
	if (m_eCameraType == CAMERATYPE::CAM_AIRCRAFT)
	{
		_matrix T;
		D3DXMatrixRotationAxis(&T, &m_vLook_D, fAngle);

		D3DXVec3TransformCoord(&m_vRight_D, &m_vRight_D, &T);
		D3DXVec3TransformCoord(&m_vUp_D, &m_vUp_D, &T);
	}
}

void CNewFPSCamera::GetViewMatrix(_matrix* V)
{
	m_vPos_D = m_vPlayerPos;

	// 카메라의 축이 서로 직각이 되도록 한다.
	D3DXVec3Normalize(&m_vLook_D, &m_vLook_D);

	D3DXVec3Cross(&m_vUp_D, &m_vLook_D, &m_vRight_D);
	D3DXVec3Normalize(&m_vUp_D, &m_vUp_D);

	D3DXVec3Cross(&m_vRight_D, &m_vUp_D, &m_vLook_D);
	D3DXVec3Normalize(&m_vRight_D, &m_vRight_D);

	// 뷰 행렬 구성
	float x = -D3DXVec3Dot(&m_vRight_D, &m_vPos_D);
	float y = -D3DXVec3Dot(&m_vUp_D, &m_vPos_D);
	float z = -D3DXVec3Dot(&m_vLook_D, &m_vPos_D);

	(*V) (0, 0) = m_vRight_D.x;
	(*V) (0, 1) = m_vUp_D.x;
	(*V) (0, 2) = m_vLook_D.x;
	(*V) (0, 3) = 0.0f;

	(*V) (1, 0) = m_vRight_D.y;
	(*V) (1, 1) = m_vUp_D.y;
	(*V) (1, 2) = m_vLook_D.y;
	(*V) (1, 3) = 0.0f;

	(*V) (2, 0) = m_vRight_D.z;
	(*V) (2, 1) = m_vUp_D.z;
	(*V) (2, 2) = m_vLook_D.z;
	(*V) (2, 3) = 0.0f;

	(*V) (3, 0) = x;
	(*V) (3, 1) = y;
	(*V) (3, 2) = z;
	(*V) (3, 3) = 1.0f;
}

void CNewFPSCamera::SetCameraType(CAMERATYPE cameraType)
{
}

void CNewFPSCamera::GetPosition(_vec3* pos)
{
}

void CNewFPSCamera::SetPosition(_vec3* pos)
{
}

void CNewFPSCamera::GetRight(_vec3* right)
{
}

void CNewFPSCamera::GetUp(_vec3* up)
{
}

void CNewFPSCamera::GetLook(_vec3* look)
{
}
