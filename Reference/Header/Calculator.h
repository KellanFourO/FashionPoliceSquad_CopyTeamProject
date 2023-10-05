#pragma once

#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CCubeTex;
class CTransform;
class CWireTex;

class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator();

	_float		Compute_HeightOnTerrain(const _vec3* pPos,
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX = VTXCNTX_DOUBLE,
		const _ulong& dwCntZ = VTXCNTZ_DOUBLE,
		const _ulong& dwVtxItv = 1);

	_vec3		Picking_OnTerrain(HWND hWnd, CTerrainTex* pTerrainBufferCom, CTransform* pTerrainTransformCom, const _ulong& dwCntX = VTXCNTX_DOUBLE, const _ulong& dwCntZ = VTXCNTZ_DOUBLE);
	_vec3		Picking_OnCube(HWND hWnd, CCubeTex* pTerrainBufferCom, CTransform* pCubeTransformCom, const vector<CUBE>* pVectorCube);
	_vec3		MapCursor_From_Mouse(HWND hWnd, CWireTex* pWireBufferCom, CTransform* pWireTransformCom, const _ulong& dwCntX = VTXCNTX_DOUBLE, const _ulong& dwCntZ = VTXCNTZ_DOUBLE);


public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

	CWireTex* m_pWireTexCom = nullptr;

private:
	virtual void Free();
};

END