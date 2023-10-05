#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSYTex : public CVIBuffer
{
private:
	explicit				CSYTex();
	explicit				CSYTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit				CSYTex(const CSYTex& rhs);
	virtual					~CSYTex();

public:
	virtual HRESULT			Ready_Buffer();
	virtual void			Render_Buffer(int _iFrame, int _iColumn);

	void					SetCount(_int _iHor, _int _iVer ) { m_iVerCount = _iVer; m_iHorCount = _iHor; }

public:
	static CSYTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone();

private:
	_float					m_fLeft			= 0.f;
	_float					m_fRight		= 0.f;
	_float					m_fTop			= 0.f;
	_float					m_fBottom		= 0.f;

	_int					m_iVerCount = 0;
	_int					m_iHorCount = 0;
private:
	virtual void			Free();
};

END

