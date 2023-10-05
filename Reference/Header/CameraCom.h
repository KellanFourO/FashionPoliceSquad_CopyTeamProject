#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCameraCom : public CComponent
{
private:
	explicit CCameraCom();
	explicit CCameraCom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCameraCom(const CCameraCom& rhs);
	virtual ~CCameraCom();

public:


public:
	HRESULT			Ready_Camera();
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

public:
	_matrix&		Get_ViewMatrix() { return m_matView; }
	_matrix&		Get_ProjMatrix() { return m_matProj; }
	void			Set_ViewSpcae();
	virtual void	Set_Projection(const CAMERA_TYPE eMode = CAMERA_TYPE::MOVE_CAMERA);

private:
	_matrix	m_matView;
	_matrix	m_matProj;

public:
	static		CCameraCom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		CComponent* Clone(void);
private:
	virtual void			Free();

	_vec3 m_vecAt;

};

END