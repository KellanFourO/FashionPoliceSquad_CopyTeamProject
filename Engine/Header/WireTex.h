#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CWireTex : public CVIBuffer
{
public:
	explicit CWireTex();
	explicit CWireTex(LPDIRECT3DDEVICE9 m_pGraphicDev);
	explicit CWireTex(const CWireTex& rhs);
	virtual ~CWireTex();

public:
	HRESULT				Ready_Buffer
						(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void		Render_Buffer();

	const	_vec3* Get_VtxPos() const { return m_pPos; }


private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_vec3*				m_pPos;

public:
	static CWireTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ulong& dwCntX = VTXCNTX_DOUBLE,
							const _ulong& dwCntZ = VTXCNTZ_DOUBLE,
							const _ulong& dwVtxItv = VTXWITV);

	virtual CComponent* Clone();

private:
	virtual void Free();
};


END