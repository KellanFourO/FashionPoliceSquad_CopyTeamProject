#include "..\Header\Gun.h"

CGun::CGun(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGun::CGun(const CGun& rhs) : CGameObject(rhs)
{
	m_pGraphicDev->AddRef();
}

CGun::~CGun()
{
	Free();
}

HRESULT CGun::Ready_GameObject()
{
	return NULL; 
}

_int CGun::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeReload += fTimeDelta;
	__super::Update_GameObject(fTimeDelta); 
	return OBJ_NOEVENT; 
}

void CGun::LateUpdate_GameObject()
{ 
	__super::LateUpdate_GameObject();
}

void CGun::Free()
{
	__super::Free();
}
