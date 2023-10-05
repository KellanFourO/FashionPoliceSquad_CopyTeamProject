#include "SYTex.h"

CSYTex::CSYTex()
{
}

CSYTex::CSYTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CSYTex::CSYTex(const CSYTex& rhs)
	: CVIBuffer(rhs)
{


	m_fLeft			= rhs.m_fLeft;
	m_fRight		= rhs.m_fRight;
	m_fTop			= rhs.m_fTop;
	m_fBottom		= rhs.m_fBottom;
}

CSYTex::~CSYTex()
{
}

HRESULT CSYTex::Ready_Buffer()
{
	m_dwFVF = VTXTEX::FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;




	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래 삼각형
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}
								// 1			 // 4
void CSYTex::Render_Buffer(int _iFrame, int _iColumn)
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_float fHorAdd = 1.f / m_iHorCount;
	_float fVerAdd = 1.f / m_iVerCount;

	m_fRight = fHorAdd * _iFrame; // 0.2
	m_fLeft = m_fRight - fHorAdd; // 0
	// LEFT ~ RIGHT 0 ~ 0.2

	m_fBottom = fVerAdd * _iColumn; // 0.8
	m_fTop = m_fBottom - fVerAdd; // 0.6
	// Top ~ Bottom = 0.6 ~ 0.8

	pVertex[0].vTexUV = { m_fLeft , m_fTop };
	pVertex[1].vTexUV = { m_fRight, m_fTop };
	pVertex[2].vTexUV = { m_fRight, m_fBottom };
	pVertex[3].vTexUV = { m_fLeft, m_fBottom };

	m_pVB->Unlock();

	//프레임 쪼게기
	CVIBuffer::Render_Buffer();
}

CSYTex* CSYTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSYTex* pInstance = new CSYTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("SYTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CSYTex::Clone(void)
{
	return new CSYTex(*this);
}

void CSYTex::Free(void)
{
	CVIBuffer::Free();
}
