
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
	_uint	iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();
}

CTexture::~CTexture()
{
	Free();
}

HRESULT CTexture::Ready_Texture(TEXTUREID eType,
	const _tchar* pPath,
	const _uint& iCnt)
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
void CTexture::Render_OBJTextrue(IDirect3DCubeTexture9* pTexture)
{
	if (pTexture == nullptr) { return; }
	m_pGraphicDev->SetTexture(0, pTexture);
}
void CTexture::Render_OBJTextrue(IDirect3DBaseTexture9* pTexture)
{
	if (pTexture == nullptr) { return; }
	m_pGraphicDev->SetTexture(0, pTexture);
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

CComponent* CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{

	__super::Free();

// 	while (!m_vecTexture.empty())
// 	{
// 		delete m_vecTexture.back();
// 		m_vecTexture.back() = nullptr;
//
// 		m_vecTexture.pop_back();
// 	}
// 	m_vecTexture.clear();
}
