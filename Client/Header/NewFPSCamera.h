#pragma once

#include "Camera.h"

class CNewFPSCamera : public Engine::CCamera
{
private:
	explicit CNewFPSCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNewFPSCamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject()	override;
	virtual void	Render_GameObject() override;

	HRESULT			Add_Component();

;

private:
	void		Check_KeyInput(const _float& fTimeDelta);

	void		Mouse_Move(const _float& fTimeDelta);
	void		Mouse_Fix();

	void		First_Camera();
	void		Third_Camera();

public:
	//_bool		Get_CameraMode() { return m_bCameraCheck; }
	_bool		Get_MouseFix() { return m_bFix; }

public:
	void		Set_Fix(_bool _Fix) { m_bFix = _Fix; }
	//void		Set_ShakeForce(_float _ShakeTime, _float _Amplitude, _float _Duration, _float _Frequency);
	void		Reset_ShakeForce();

public:
	void		Shake_Camera();
	void		Drunk_Camera();

private:
	// Camera Shake
	_bool		m_bShaking; // ��鸲 ����
	_float		m_fShakeElipsedTime; // ��ø�ð�

	_float		m_fAmplitude; // ����
	_float		m_fDuration;  // ���� �ð�
	_float		m_fFrequency; // ���� �ӵ�

	_vec3		m_vOriginPos; // ���� �ִ� ��ġ

	CAMERAID	m_eCamera_Mode; // ī�޶� ��� ����
	bool		m_bCameraCheck;

private:
	_vec3		vLook = { 0.f,0.f,1.f };
	_vec3		vUp = { 0.f, 1.f, 0.f };

	_float		m_fSpeed = 30.f;
	_bool		m_bFix = true;

	_long		m_dwMouseMove[ROT_END];

	_float		m_fMinAngleY;
	_float		m_fMaxAngleY;
	_float		m_fAngleX;
	_float		m_fAngleY;

	CTransform* pPlayerTransCom = nullptr;
	_vec3		m_vPlayerPos;

	_vec3		m_vRight;
	_quat		m_quaternion;

//////// ��å ������ ī�޶� ////////////////////
public:
	enum class CAMERATYPE { CAM_LAND_OBJ , CAM_AIRCRAFT, CAMTYPE_END };

	void Strafe(float units); 	//����/������
	void Fly(float units); 		//����/�Ʒ���
	void Walk(float units); 	//����/����

	void Pitch(float fAngle); 	// ���� ���ͷ� ȸ��
	void Yaw(float fAngle); 		// ���� ���ͷ� ȸ��
	void Roll(float fAngle); 	// ���� ���ͷ� ȸ��

	void GetViewMatrix(_matrix* V);
	void SetCameraType(CAMERATYPE CameraType);
	void GetPosition(_vec3* pPos);
	void SetPosition(_vec3* pPos);
	void GetRight(_vec3* pRight);
	void GetUp(_vec3* pUp);
	void GetLook(_vec3* pLook);

private:
	CAMERATYPE	m_eCameraType;
	_vec3		m_vRight_D;
	_vec3		m_vUp_D;
	_vec3		m_vLook_D;
	_vec3		m_vPos_D;

/////////////////////////////////////////////

public:
	static CNewFPSCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
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

