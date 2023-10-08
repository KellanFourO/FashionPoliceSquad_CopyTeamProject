#include "Transform.h"

CTransform::CTransform()
	: m_vAngle(0.f, 0.f, 0.f), m_vScale(1.f, 1.f, 1.f) ,m_pParent(nullptr) , m_vAxis(0.f,0.f,0.f),m_fAxisAngle(0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);

}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_vAngle(0.f, 0.f, 0.f), m_vScale(1.f, 1.f, 1.f) ,m_pParent(nullptr), m_vAxis(0.f, 0.f, 0.f), m_fAxisAngle(0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}


CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_vAngle(rhs.m_vAngle), m_vScale(rhs.m_vScale),m_pParent(rhs.m_pParent),m_pChild(rhs.m_pChild)
	, m_vAxis(0.f, 0.f, 0.f), m_fAxisAngle(0.f)
{
	for (size_t i = 0; i < INFO_END; ++i)
			m_vInfo[i] = rhs.m_vInfo[i];

	m_pChild = rhs.m_pChild;
	m_matWorld = rhs.m_matWorld;
	
}

CTransform::~CTransform()
{
	//Free();
}




HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld); // 항등초기화

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld); // 항등행렬



	for (_int i = 0; i < INFO_POS; ++i) // INFO_POS까지 도는게 포인트
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	for (_int i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	_matrix matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	if (m_vAxis == _vec3{ 0.f,0.f,0.f }) {
		D3DXMatrixIdentity(&matRot[ROT_AXIS]);
	}
	else {
		D3DXMatrixRotationAxis(&matRot[ROT_AXIS],&m_vAxis,m_fAxisAngle);
	}

	for (_int i = 0; i < INFO_POS; ++i)
	{
		for (_int j = 0; j < ROT_END; ++j)
		{

			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);

		}

	}

	// 아래 코드상의  m_vInfo는 크기변환과 회전변환 연산이 끝난 상태
	// 벡터의 방향성

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}




void CTransform::Move_Pos(const _vec3* const pDir, const _float& fTimeDelta, const _float& fSpeed)
{
	m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed;
}

void CTransform::Move_Terrain(const _vec3* pPos, const _float& fTimeDelta, const _float& fSpeed)
{
	_vec3	vDir = *pPos - m_vInfo[INFO_POS];
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;
}

_vec3* CTransform::Get_Scale()
{
// 	for (_int i = 0; i < INFO_POS; ++i)
// 		*(((_float*)&m_vScale) + i) = D3DXVec3Length(&m_vInfo[i]);
	return &m_vScale;

}




const _matrix CTransform::WorldMatrix()
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	for (int i = 0; i < INFO_END; ++i)
		::CopyMemory(&matWorld.m[i], m_vInfo[i], sizeof(_vec3));

	return matWorld;
}

void	CTransform::Copy_RUL(_vec3* _Info)
{
	for (_uint i = 0; i < INFO_POS; ++i)
		CopyMemory(m_vInfo[i], _Info[i], sizeof(_vec3));
}

void	CTransform::Copy_RUL_AddPos(_vec3* _Info)
{
	for (_uint i = 0; i < INFO_END; ++i)
		CopyMemory(m_vInfo[i], _Info[i], sizeof(_vec3));
}




void CTransform::Translate(_vec3& _vTranslation)
{
	m_vInfo[INFO_POS] += _vTranslation;
	if (!m_pChild.empty())
	{
		for (auto& iter : m_pChild)
			iter->Translate(_vTranslation);
	}
}

void CTransform::Translate(const _vec3& _vTranslation)
{
	Translate(const_cast<_vec3&>(_vTranslation));
}




void CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

void CTransform::Set_Rotate(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) = fAngle;
	m_vAngle.y;
}




_matrix& CTransform::Rotate(_vec3& _vEulers)
{
	_matrix matRotate;
	//_quat quat;
	//_vec3 vRotAxis;

	D3DXQuaternionIdentity(&m_quat);

	if (0.f != _vEulers.y)
		D3DXQuaternionRotationAxis(&m_quat, &m_v, _vEulers.y);
	else if (0.f != _vEulers.x)
		D3DXQuaternionRotationAxis(&m_quat, &m_vInfo[INFO_RIGHT], _vEulers.x );
	else if (0.f != _vEulers.z)
		D3DXQuaternionRotationAxis(&m_quat, &m_vInfo[INFO_LOOK], _vEulers.z);
	

	D3DXMatrixRotationQuaternion(&matRotate, &m_quat);

	for (int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);
	
	if (!m_pChild.empty())
	{
		if (0.f != _vEulers.y)
			for (auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], _vec3(0.f, 1.f, 0.f), _vEulers.y);
		if (0.f != _vEulers.x)
			for (auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], m_vInfo[INFO_RIGHT], _vEulers.x);
		if (0.f != _vEulers.z)
			for (auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], m_vInfo[INFO_LOOK], _vEulers.z);
	}

	return matRotate;
}
 
_matrix& CTransform::Rotate(const _vec3& _vEulers)
{
	return Rotate(const_cast<_vec3&>(_vEulers));
}

void CTransform::Rotate(const _float& _fXangle, const _float& _fYangle, const _float& _fZangle)
{
	Rotate(_vec3(_fXangle, _fYangle, _fZangle));
}

void CTransform::Rotate(ROTATION eType, const _float& fAngle)
{
	_vec3 vAngle = _vec3(0.f, 0.f, 0.f);
	*(((_float*)&vAngle) + eType) += fAngle;

	Rotate(_vec3(vAngle[INFO_RIGHT], vAngle[INFO_UP], vAngle[INFO_LOOK]));
}

void CTransform::RotateAround(const _vec3& _vPoint, const _vec3& _vAxis, const _float& _fAngle)
{
	_quat quat;
	_matrix matRotate;
	_vec3 vOut;
	m_AroundV = m_vInfo[INFO_POS] - _vPoint;
	// D3DXMatrixRotationAxis(&matRotate, &_vAxis, _fAngle);
	D3DXQuaternionRotationAxis(&quat, &_vAxis, _fAngle);
	D3DXMatrixRotationQuaternion(&matRotate, &quat);
	D3DXVec3TransformCoord(&vOut, &m_AroundV, &matRotate);
	for (int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);

	m_vInfo[INFO_POS] = vOut + _vPoint;
}

void CTransform::RotateAxis(_vec3& _Axis, _float _AxisAngle)
{
	m_vAxis = _Axis;
	m_fAxisAngle = _AxisAngle;
}





void CTransform::Chase_Target(const _vec3* _pTargetPos, const _float& _fTimeDelta, const _float& _fSpeed)
{
	_vec3	vDir = *_pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * _fTimeDelta * _fSpeed;
	
	_matrix matTrans;
	_matrix matRot = *Compute_LootAtTarget(_pTargetPos);
	
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);


	m_matWorld = matRot * matTrans;
}

_vec3 CTransform::LocalScale()
{
	_vec3 vScale;
	for (_int i = 0; i < INFO_POS; ++i)
		*(((_float*)&vScale) + i) = D3DXVec3Length(&m_vInfo[i]);

	return vScale;
}





const _matrix* CTransform::Compute_LootAtTarget(const _vec3* _pTargetPos)
{
	_vec3	vDir = *_pTargetPos - m_vInfo[INFO_POS];

	_vec3 vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_vec3 vUp;

	_matrix matRot;

	return D3DXMatrixRotationAxis(&matRot, &vAxis, acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform())) // 이니셜라이즈가 실패한경우
	{
		Safe_Release(pInstance);
		MSG_BOX("Transform Component Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	if (m_pParent)
	{
		for_each(m_pChild.begin(), m_pChild.end(), CDeleteObj());
		m_pChild.clear();
	}
	CComponent::Free();
}
