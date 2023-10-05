#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:

	HRESULT			Ready_Transform();
	virtual _int	Update_Component(const _float& fTimeDelta);


	void			Move_Pos(const _vec3* const pDir, const _float& fTimeDelta, const _float& fSpeed);
	void			Move_Terrain(const _vec3* pPos, const _float& fTimeDelta, const _float& fSpeed);

	void			Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
					{	m_vInfo[INFO_POS] = { fX, fY, fZ }; }
	void			Set_Pos(const _vec3& vPos)
					{	m_vInfo[INFO_POS] = vPos;	 }


	void			Get_Info(INFO eType, _vec3* pInfo) { memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3)); }
	void			Set_Look(const _vec3& pInfo)	{	m_vInfo[INFO_LOOK] = pInfo;  }

	_vec3*			Get_Scale();
	void			Set_Scale(const _vec3& vScale)	{	m_vScale = vScale;	}

// WorldMatrix 관련 함수/////////////
	void			Set_WorldMatrix(const _matrix* pWorld) 	{ m_matWorld = *pWorld; }
	_matrix*		Get_WorldMatrix() { return &m_matWorld; }
	const _matrix	WorldMatrix();

// FlyCamera용///////////////////////
	void			Copy_RUL(_vec3* _Info);
	void			Copy_RUL_AddPos(_vec3* _Info);

// TransLate 함수////////////////////
	void			Translate(_vec3& _vTranslation);
	void			Translate(const _vec3& _vTranslation);

// 회전 관련 함수////////////////////
	void			Rotation(ROTATION eType, const _float& fAngle);
	void			Set_Rotate(ROTATION eType, const _float& fAngle);
	_matrix&		Rotate(_vec3 & _vEulers);
	_matrix&		Rotate(const _vec3& _vEulers);
	void			Rotate(const _float& _fXangle, const _float& _fYangle, const _float& _fZangle);
	void			Rotate(ROTATION eType, const _float& fAngle);
	void			RotateAround(const _vec3& _vPoint, const _vec3& _vAxis, const _float& _fAngle);
	void			RotateAxis(_vec3& _Axis, _float _AxisAngle); // 임의의 축 회전 추가 (재연)

	void			Chase_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _float& fSpeed);
	_vec3			LocalScale();
//////////////////////////////////////
private:
	const _matrix*	Compute_LootAtTarget(const _vec3* pTargetPos);

	void			Set_Child(CTransform* _pChild) { m_pChild.push_back(_pChild); }

public:
	vector<CTransform*>		m_pChild;
	_vec3					m_vInfo[INFO_END];
	_vec3					m_vScale;
	_vec3					m_vAngle;
	_matrix					m_matWorld;
	CTransform*				m_pParent;
	_quat					m_quat;
	_vec3					m_vAxis;
	_float					m_fAxisAngle;	// 임의의 축 회전 추가 -재연

private:
	_vec3					m_v = _vec3(0.f, 1.f, 0.f);
	_vec3					m_AroundV;
public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone();

private:
	virtual void			Free();

};

END