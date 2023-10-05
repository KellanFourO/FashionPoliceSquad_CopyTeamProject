#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CUITex : public CVIBuffer
{
private:
	explicit				CUITex();
	explicit				CUITex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit				CUITex(const CUITex& rhs);
	virtual					~CUITex();

public:
	_float  GetRatio() { return m_fRatio; }
	void	SetRatio(_float fRatio) { m_fRatio = fRatio; }
	_float  GetMax() { return m_fMax; }
	void	SetMax(_float fMax) { m_fMax = fMax; }

public:
	virtual HRESULT			Ready_Buffer();
	virtual void			Render_Buffer();

public:
	static CUITex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();

private:
	_float					m_fRatio = 1.f;
	_float					m_fMax = 0.f;
};

END

