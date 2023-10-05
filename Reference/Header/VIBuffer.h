#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong			m_dwVtxCnt;			// 정점의 개수
	_ulong			m_dwVtxSize;		// 정점의 크기
	_ulong			m_dwTriCnt;			// 삼각형의 개수
	_ulong			m_dwFVF;			// 정점의 속성

	_ulong			m_dwIdxSize;
	D3DFORMAT		m_IdxFmt;

public:
	void					Get_Frame(_float _fVerDevide, _float _fHorDevide, _float _fFrame);
	void					Get_WaterFrame(_float _fWaterFrame) { m_fWaterFrame = _fWaterFrame; }

public:
	_float					m_fVerDevide = 1; // 프레임 좌우 갯수
	_float					m_fHorDevide = 1; // 프레임 상하 갯수
	_float					m_fFrame = 0;	//출력 할 프레임
	_float					m_fWaterFrame = 0; // 물의 프레임. 20프레임까지로 나눠져있음.




public:
	virtual void Free();
};

END