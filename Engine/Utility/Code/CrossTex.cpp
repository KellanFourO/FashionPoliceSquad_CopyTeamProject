
#include "CrossTex.h"


CCrossTex::CCrossTex()
{
}

CCrossTex::CCrossTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CCrossTex::CCrossTex(const CCrossTex& rhs)
	: CVIBuffer(rhs)
{
}

CCrossTex::~CCrossTex()
{
}

HRESULT CCrossTex::Ready_Buffer()
{
	m_dwFVF = VTXCROSS::FVF_CROSS;
	m_dwTriCnt = 4; // 2면 크로스
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCROSS);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCROSS* m_pVertex = nullptr;

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

	m_pVB->Unlock();



	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +X 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	// +X 오른쪽 아래 
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 3;

	// -X 오른쪽 위l
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 1;
	pIndex[2]._2 = 2;

	// -X 왼쪽 아래
	pIndex[3]._0 = 4;
	pIndex[3]._1 = 2;
	pIndex[3]._2 = 7;


	m_pIB->Unlock();

	return S_OK;
}

void CCrossTex::Render_Buffer(void)
{

	CVIBuffer::Render_Buffer();
}

CCrossTex* CCrossTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossTex* pInstance = new CCrossTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CCubeTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CCrossTex::Clone(void)
{
	return new CCrossTex(*this);
}

void CCrossTex::Free(void)
{

	CVIBuffer::Free();

}
