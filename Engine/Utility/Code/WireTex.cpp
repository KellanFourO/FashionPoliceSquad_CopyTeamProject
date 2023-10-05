#include "WireTex.h"

CWireTex::CWireTex()
{
}

CWireTex::CWireTex(LPDIRECT3DDEVICE9 m_pGraphicDev)
	: CVIBuffer(m_pGraphicDev), m_pPos(nullptr)
{
}

CWireTex::CWireTex(const CWireTex& rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_pPos(rhs.m_pPos)
{
}

CWireTex::~CWireTex()
{
}

HRESULT CWireTex::Ready_Buffer
(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwFVF = VTXTEX::FVF_TEX;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_pPos = new _vec3[m_dwVtxCnt]; //버텍스 갯수만큼 pos 배열
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;
	INDEX32*	pIndex = nullptr;


	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong dwIndex(0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = { _float(j) * dwVtxItv,
				0.f * dwVtxItv, _float(i) * dwVtxItv };

			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };
			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;

			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1),
										_float(i) / (dwCntZ - 1) };
		}
	}

	_ulong		dwTriIdx(0);
	_vec3		vDst, vSrc, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			//vDst = pVertex[pIndex[dwTriIdx]._1].vPosition - pVertex[pIndex[dwTriIdx]._0].vPosition;
			//vSrc = pVertex[pIndex[dwTriIdx]._2].vPosition - pVertex[pIndex[dwTriIdx]._1].vPosition;
			//D3DXVec3Cross(&vNormal, &vDst, &vSrc);
			//
			//pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			//pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			//pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			dwTriIdx++;

			// 왼쪽 아래 삼각형
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			//vDst = pVertex[pIndex[dwTriIdx]._1].vPosition - pVertex[pIndex[dwTriIdx]._0].vPosition;
			//vSrc = pVertex[pIndex[dwTriIdx]._2].vPosition - pVertex[pIndex[dwTriIdx]._1].vPosition;
			//D3DXVec3Cross(&vNormal, &vDst, &vSrc);
			//
			//pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			//pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			//pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			dwTriIdx++;
		}
	}

	//for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	//	D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();


	return S_OK;
}

void CWireTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CWireTex* CWireTex::Create
(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CWireTex* pInstance = new CWireTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("WireTex Create Failed");
		return nullptr;
	}
	return pInstance;
}

CComponent* CWireTex::Clone()
{
	return new CWireTex(*this);
}

void CWireTex::Free()
{
	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);

	CVIBuffer::Free();
}
