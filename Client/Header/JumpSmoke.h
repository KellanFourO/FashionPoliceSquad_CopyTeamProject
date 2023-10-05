#pragma once

#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "PSystem.h"
#include "OctreeNode.h"
BEGIN(Engine)

class CTransform;

END
class CJumpSmoke : public CPSystem
{
private:
	explicit CJumpSmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CJumpSmoke();
public:
	HRESULT							Ready_GameObject(_vec3 vOriginPos, _int numParticles);
	_int							Update_GameObject(const _float& fTimeDelta);
	void							LateUpdate_GameObject(void);
	void							Render_GameObject();

	static CJumpSmoke*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles);

private:
	virtual void					ResetParticle(Attribute* attribute);
	HRESULT							Add_Component();
	void							billboard();
private:
	_float	    m_fTime = 0.f;
	_float      m_fMoveTime = 20.f;
	_float		m_fDieTime = 20.f;
private:
	virtual void Free() override;
};

