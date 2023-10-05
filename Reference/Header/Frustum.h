#pragma once
#include "Base.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CCamera;
class ENGINE_DLL CFrustum : public CBase
{
public:
	explicit CFrustum();
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum();

public:
	void	MakeFrustumView(_matrix* _pmatProj);
	BOOL	MakeFrustum(_matrix* _pmatWorld);
	_int	Update_Frustum();

	BOOL	IsIn(_vec3& pv);
	BOOL	IsInSphere(_vec3& pv, _float _fRadius);

private:
	_vec3		m_vtx[8];
	_vec3		m_vtxView[8];
	_vec3		m_vtxWorld[8];
	D3DXPLANE	m_tPlane[6];

	CGameObject* m_pCamera = nullptr;
	

public:
	virtual void Free() override;

};

END