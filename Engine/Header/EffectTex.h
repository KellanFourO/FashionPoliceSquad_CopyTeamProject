#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CEffectTex : public CVIBuffer
{
private:
	explicit CEffectTex();
	explicit CEffectTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffectTex(const CEffectTex& rhs);
	virtual ~CEffectTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	static CEffectTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone();

private:
	virtual void Free();
};

END