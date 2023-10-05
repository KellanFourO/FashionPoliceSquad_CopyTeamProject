#include "CubeTex.h"

CCubeTex::CCubeTex()
{
}

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CCubeTex::CCubeTex(const CCubeTex& rhs)
	: CVIBuffer(rhs)
{

}

CCubeTex::~CCubeTex()
{
}

HRESULT CCubeTex::Ready_Buffer(void)
{
	m_dwFVF = VTXCUBE::FVF_CUBE;
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12; // 6면
	m_dwVtxSize = sizeof(VTXCUBE);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE* m_pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&m_pVertex, 0);

	// 

	m_pVertex[0].vPosition = { -1.f, 1.f, -1.f };
	m_pVertex[0].vTexUV = m_pVertex[0].vPosition;

	m_pVertex[1].vPosition = { 1.f, 1.f, -1.f };
	m_pVertex[1].vTexUV = m_pVertex[1].vPosition;

	m_pVertex[2].vPosition = { 1.f, -1.f, -1.f };
	m_pVertex[2].vTexUV = m_pVertex[2].vPosition;

	m_pVertex[3].vPosition = { -1.f, -1.f, -1.f };
	m_pVertex[3].vTexUV = m_pVertex[3].vPosition;

	m_pVertex[4].vPosition = { -1.f, 1.f, 1.f };
	m_pVertex[4].vTexUV = m_pVertex[4].vPosition;

	m_pVertex[5].vPosition = { 1.f, 1.f, 1.f };
	m_pVertex[5].vTexUV = m_pVertex[5].vPosition;

	m_pVertex[6].vPosition = { 1.f, -1.f, 1.f };
	m_pVertex[6].vTexUV = m_pVertex[6].vPosition;

	m_pVertex[7].vPosition = { -1.f, -1.f, 1.f };
	m_pVertex[7].vTexUV = m_pVertex[7].vPosition;

	
	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	// +X 오른쪽 위
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;
		
	// +X 오른쪽 아래 
	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;
			
	// -X 오른쪽 위
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	// -X 왼쪽 아래
	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// +Y 오른쪽 위
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;


	// +Y 왼쪽 아래
	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// -Y 오른쪽 위
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	// -Y 왼쪽 아래 
	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// +Z 오른쪽 위
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	// +Z 왼쪽 아래
	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// -Z 오른쪽 위
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	// -Z 왼쪽 아래
	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;
	

	//삼각형 평면 기준
	for (_ulong i = 0; i < m_dwTriCnt; ++i) 
 	{
    		_ulong i0 = pIndex[i]._0;
    		_ulong i1 = pIndex[i]._1;
    		_ulong i2 = pIndex[i]._2;

			_vec3		vDst, vSrc, vNormal;  

			vDst = m_pVertex[i1].vPosition - m_pVertex[i0].vPosition; 
			vSrc = m_pVertex[i2].vPosition - m_pVertex[i1].vPosition; 
			D3DXVec3Cross(&vNormal, &vDst, &vSrc);													//

			m_pVertex[i0].vNormal += vNormal;
			m_pVertex[i1].vNormal += vNormal;
			m_pVertex[i2].vNormal += vNormal;
 	}

	//정규화
		for (DWORD i = 0; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3Normalize(&m_pVertex[i].vNormal, &m_pVertex[i].vNormal);
	}


	m_pIB->Unlock();
	m_pVB->Unlock();
	return S_OK;
}

void CCubeTex::Render_Buffer(void)
{

	CVIBuffer::Render_Buffer();
}

CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTex* pInstance = new CCubeTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CCubeTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CCubeTex::Clone(void)
{
	return new CCubeTex(*this);
}

void CCubeTex::Free(void)
{

	CVIBuffer::Free();

}
