#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider();
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();
public:
	HRESULT			Ready_Collider();
	virtual void    LateUpdate_Component() override;
	void			Render_Collider();

	HRESULT			InitOBB(_vec3& _vCenter, _vec3* _vAxisDir, _float* _fAxisLen);

	_vec3&			GetCenterPos()	{ return m_vCenterPos; }
	_vec3*			GetAxisDir() { return m_vAxisDir; }
	float*			GetAxisLen() { return m_fAxisLen; }

	void			Set_OBJAttribute(OBJ_ATTRIBUTE eAttri) { m_eAttribute = eAttri; }
	OBJ_ATTRIBUTE	Get_OBJAttribute() { return m_eAttribute; }

	UINT			GetID() { return m_iID; }

	void			SetCenterPos(_vec3& _vCenterPos) { m_vCenterPos = _vCenterPos; }
public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:

	_vec3		m_vCenterPos; //상자 중앙 좌표
	_vec3		m_vAxisDir[3]; // 상자에 평행한 세축의 단위 벡터
	_float		m_fAxisLen[3]; // 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.

	_uint		m_iID;
	static _uint g_iNextID;

	int			m_iCol;

	//유진, 계단 검출용으로 사용함.
	OBJ_ATTRIBUTE	m_eAttribute = OBJ_ATTRIBUTE::ATTRIBUTE_END;

	//디버그에서만 사용함
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();
private:
	virtual void	Free();
};

END