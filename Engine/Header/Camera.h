#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	LateUpdate_GameObject() override;
	
	virtual void	Render_GameObject() {}

public:
	_vec3			Get_Eye() { return m_vEye; }
	void			Set_Eye(_vec3 vEye) { m_vEye = vEye; }
	void			Set_At(_vec3 vAt) { m_vAt = vAt; }
	void			Set_Up(_vec3 vUp) { m_vUp = vUp; }

	_matrix			Get_View() { return m_matView; }
	_matrix			Get_Proj() { return m_matProj; }


	void			Make_ViewMatrix(D3DXMATRIX* pOut, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	void			Make_PerspectiveMatrix(D3DXMATRIX* pOut, float fFovy, float fAspect, float fNear, float fFar);

protected:
	_vec3			m_vEye, m_vAt, m_vUp; // �ڱ��ڽ��� �þ�, �ٶ󺸴� ���, �ڱ��ڽ��� ���⺤�� (0,1,0)
	_float			m_fFov, m_fAspect, m_fNear, m_fFar; // y������ �����ʵ�(����), ���μ��κ���
	_matrix			m_matView, m_matProj;
	
protected:
	virtual void	Free();
};

END
