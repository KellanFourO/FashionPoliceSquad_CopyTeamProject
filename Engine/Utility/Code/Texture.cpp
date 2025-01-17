
#include "..\..\Header\Texture.h"

CTexture::CTexture()
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{

	// 텍스쳐 메인 vector 복사하기
	_uint	iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();


	// 유진 OBJ 텍스쳐 vector 복사하기
	_uint	iSize2 = rhs.m_vecCubeTexture.size();
	m_vecCubeTexture.reserve(iSize2);

	m_vecCubeTexture = rhs.m_vecCubeTexture;

	for (_uint i = 0; i < iSize2; ++i)
		m_vecCubeTexture[i]->AddRef();


	_uint	iSize3 = rhs.m_vecPlaneTexture.size();
	m_vecPlaneTexture.reserve(iSize3);

	m_vecPlaneTexture = rhs.m_vecPlaneTexture;

	for (_uint i = 0; i < iSize3; ++i)
		m_vecPlaneTexture[i]->AddRef();


}

CTexture::~CTexture()
{
	Free();
}

////////////////////// Main 텍스쳐 함수 ////////////////////////////////


HRESULT CTexture::Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9* pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR	szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TEX_NORMAL:


			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:

			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}
		m_vecTexture.push_back(pTexture);
	}
	return S_OK;
}

void CTexture::Render_Textrue(const _uint& iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, _tchar* pPath, const _uint& iCnt)
{
	CTexture* pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(eType, pPath, iCnt)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Texture Create Failed");
		return nullptr;
	}

	return pInstance;
}



////////////////////////// 유진 OBJ 텍스쳐 함수 ///////////////////////////////////


HRESULT CTexture::Ready_Texture(TEXTUREID eType,
	const _tchar* pPath, const _uint& iCnt, OBJ_TYPE eOBJType)
{
	if (eOBJType == OBJ_TYPE::CUBE_TYPE)
	{
		m_vecCubeTexture.reserve(iCnt);
		IDirect3DCubeTexture9* pTexture = nullptr;

		for (_uint i = 0; i < iCnt; ++i)
		{
			TCHAR	szFileName[256] = L"";
			wsprintf(szFileName, pPath, i);

			switch (eType)
			{
			case TEX_NORMAL:
				FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
				break;

			case TEX_CUBE:

				FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
				break;
			}
			m_vecCubeTexture.push_back(pTexture);
		}
	}
	else if (eOBJType == OBJ_TYPE::PLANE_TYPE)
	{
		m_vecPlaneTexture.reserve(iCnt);
		IDirect3DBaseTexture9* pTexture = nullptr;

		for (_uint i = 0; i < iCnt; ++i)
		{
			TCHAR	szFileName[256] = L"";
			wsprintf(szFileName, pPath, i);

			switch (eType)
			{
			case TEX_NORMAL:
				FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
				break;

			case TEX_CUBE:

				FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
				break;
			}
			m_vecPlaneTexture.push_back(pTexture);
		}

	}
	return S_OK;
}


void CTexture::Render_ObjCubeTex(const _uint& iIndex)
{
	if (m_vecCubeTexture.size() <= iIndex - cubeTextureStartIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecCubeTexture[iIndex - cubeTextureStartIndex]);
}
void CTexture::Render_ObjPlaneTex(const _uint& iIndex)
{
	if (m_vecPlaneTexture.size() <= iIndex - planeTextureStartIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecPlaneTexture[iIndex - planeTextureStartIndex]);
}



CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, _tchar* pPath, OBJ_TYPE eOBJType, const _uint& iCnt)
{
	CTexture* pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(eType, pPath, iCnt, eOBJType)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Texture Create Failed");
		return nullptr;
	}

	return pInstance;
}

////////////////////////////////////////////////////////////////////////////



CComponent* CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{

	__super::Free();

}
