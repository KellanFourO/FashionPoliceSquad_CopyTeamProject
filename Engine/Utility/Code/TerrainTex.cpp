#include "..\..\Header\TerrainTex.h"

CTerrainTex::CTerrainTex()
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_pPos(rhs.m_pPos)
{
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	m_dwFVF = VTXTEX::FVF_TEX;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	
	// ���� - Terrain ���忡 ���� ���� ������� ������Ʈ
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwVtxItv = dwVtxItv;
	//

	m_pPos = new _vec3[m_dwVtxCnt]; // ���ؽ� ī��Ʈ��ŭ �Ҵ�

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL); 
	// ���� ���õ� ���Ŀ�

#pragma region Terrain ���̸� �����ϴ� Part
	//m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	// m_hFile�� �ڵ�� ����� ��������̴�. m_hFile�� ������ �о�� ������ ��Ƴ��´�.
	//
	//_ulong dwByte = 0;
	//ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	//ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	//// ��Ʈ���� FILEHEADER, INFOHEADER, PIXEL �� ������ �ִµ� �츮�� �ʿ��� ������ PIXEL �����̴� ���� �� �κ��� �о�� ���� PIXEL �κ��� ���� �����ؼ� ���

	//_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight]; // �о�� ���� ���� ���̴� INFOHEADER�� �������־ ���̸�ŭ �о�� �ȼ����� ����� ������

	//ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, nullptr); // �ȼ�������ŭ Ȯ���س��� pPixel ������ Pixel ������ �־���

	//CloseHandle(m_hFile); // pPixel�� �츮�� �ʿ��� ������ �� ������� ������ �ݾ��ش�.
#pragma endregion

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong	dwIndex(0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = { _float(j) * dwVtxItv,
				/*_float(pPixel[dwIndex] & 0x000000ff) / 20.f,*/
				0.f,
				_float(i) * dwVtxItv };

			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;

			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 20, _float(i) / (dwCntZ - 1) * 20};
		}
	}

	m_pVB->Unlock();

// 	Safe_Delete_Array(pPixel); // ���ؽ��� �о�� �ȼ������� �־����� �뵵�� ���� pPixel ������ �����ش�.

	INDEX32* pIndex = nullptr;

	_ulong		dwTriIdx(0);

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			int index = i * dwCntX + j;

			// ������ ��
			pIndex[dwTriIdx]._0 = index + dwCntX;
			pIndex[dwTriIdx]._1 = index + dwCntX + 1;
			pIndex[dwTriIdx]._2 = index + 1;
			dwTriIdx++;
			
			// ���� �Ʒ� �ﰢ��
			pIndex[dwTriIdx]._0 = index + dwCntX;
			pIndex[dwTriIdx]._1 = index + 1;
			pIndex[dwTriIdx]._2 = index;
			dwTriIdx++;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CTerrainTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}


CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("TerrainTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free(void)
{
	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);

	// �� ������ ���� ���簡 �Ǵ� ��찡 �־�, ������ ������ �͸� �������ش�.

	CVIBuffer::Free();
}
