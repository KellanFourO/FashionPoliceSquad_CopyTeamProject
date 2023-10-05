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
	
	// 유진 - Terrain 저장에 쓰기 위해 멤버변수 업데이트
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwVtxItv = dwVtxItv;
	//

	m_pPos = new _vec3[m_dwVtxCnt]; // 버텍스 카운트만큼 할당

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL); 
	// 버퍼 셋팅된 이후에

#pragma region Terrain 높이맵 설정하는 Part
	//m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	// m_hFile은 핸들로 선언된 멤버변수이다. m_hFile에 파일을 읽어온 정보를 담아놓는다.
	//
	//_ulong dwByte = 0;
	//ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	//ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	//// 비트맵은 FILEHEADER, INFOHEADER, PIXEL 세 영역이 있는데 우리가 필요한 영역은 PIXEL 영역이니 위에 두 부분을 읽어온 이후 PIXEL 부분을 따로 저장해서 사용

	//_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight]; // 읽어온 가로 세로 길이는 INFOHEADER가 가지고있어서 길이만큼 읽어올 픽셀영역 사이즈를 맞춰줌

	//ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, nullptr); // 픽셀영역만큼 확보해놓은 pPixel 공간에 Pixel 정보를 넣어줌

	//CloseHandle(m_hFile); // pPixel의 우리가 필요한 정보는 다 얻었으니 파일을 닫아준다.
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

// 	Safe_Delete_Array(pPixel); // 버텍스의 읽어온 픽셀정보를 넣었으니 용도를 다한 pPixel 정보를 지워준다.

	INDEX32* pIndex = nullptr;

	_ulong		dwTriIdx(0);

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			int index = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriIdx]._0 = index + dwCntX;
			pIndex[dwTriIdx]._1 = index + dwCntX + 1;
			pIndex[dwTriIdx]._2 = index + 1;
			dwTriIdx++;
			
			// 왼쪽 아래 삼각형
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

	// 다 날리면 얕은 복사가 되는 경우가 있어, 원본을 제외한 것만 삭제해준다.

	CVIBuffer::Free();
}
