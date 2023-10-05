#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{

private:
	explicit				CTerrainTex();
	explicit				CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit				CTerrainTex(const CTerrainTex& rhs);
	virtual					~CTerrainTex();

public:
	HRESULT					Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void			Render_Buffer();

	const	_vec3*			Get_VtxPos() const { return m_pPos; }
	
	//유진 - Terrain 파일 입출력을 위한 Get 함수
	_ulong					Get_dwCntX() { return m_dwCntX; }
	_ulong					Get_dwCntZ() { return m_dwCntZ; }
	_ulong					Get_dwVtxItv() { return m_dwVtxItv; }
	void					Set_TerrainParameter(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
	{
		m_dwCntX = dwCntX;
		m_dwCntZ = dwCntZ;
		m_dwVtxItv = dwVtxItv;
	}


private:
	HANDLE					m_hFile;
	BITMAPFILEHEADER		m_fH;
	BITMAPINFOHEADER		m_iH;

	_vec3*					m_pPos;

public:  //유진 - Terrain 전용 /  파일 입출력을 위한 멤버변수 선언
	_ulong					m_dwCntX = 0;
	_ulong					m_dwCntZ = 0;
	_ulong					m_dwVtxItv = 0;
	

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv);
	virtual CComponent* Clone();

private:
	virtual void			Free();
};

END

