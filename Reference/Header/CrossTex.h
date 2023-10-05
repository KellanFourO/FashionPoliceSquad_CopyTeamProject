#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCrossTex : public CVIBuffer
{
public:
	explicit CCrossTex();
	explicit CCrossTex(LPDIRECT3DDEVICE9 m_pGraphicDev);
	explicit CCrossTex(const CCrossTex& rhs);
	virtual ~CCrossTex();

public:
	HRESULT				Ready_Buffer();
	virtual void		Render_Buffer();

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

public:
	static CCrossTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual CComponent* Clone();

private:
	virtual void Free();

};


END