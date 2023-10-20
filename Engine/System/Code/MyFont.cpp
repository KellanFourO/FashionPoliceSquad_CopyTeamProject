#include "..\..\Header\MyFont.h"

CMyFont::CMyFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
	, m_fTextSpeed(1.0f)
	, m_fFrame(0.0f)
	, m_nTextIndex(0)
{
	m_pGraphicDev->AddRef();
}

CMyFont::~CMyFont()
{
}

HRESULT CMyFont::Ready_Font(const _tchar * pFontType,
							const _uint & iWidth,
							const _uint & iHeight,
							const _uint & iWeight)
{
	D3DXFONT_DESC		Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGEUL_CHARSET;
	Font_Desc.Width  = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;
	lstrcpy(Font_Desc.FaceName, pFontType);

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}


void CMyFont::Render_Font(const _tchar * pString, const _vec2 * pPos, D3DXCOLOR Color, _int maxWidth, _float _fTextSpeed, _bool _bAnimation)
{
	RECT rc{ (_long)pPos->x, (_long)pPos->y };

	//TODO �¿� �ؽ�Ʈ �Ű������� ���� maxWidth���� ���� ���� �Ѱų�, '\n'�� �������� ���� �ڵ�
	//TODO �Ű������� ���� ���ڼӵ��� ���� �ؽ�Ʈ�ִϸ��̼��� �ϰ�;����� �ð��̾����� ����

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	_int iLineHeight = 30; //! �� ����
	const _tchar* pCurrentLine = pString; //! ���� ���ڿ�


	while (*pCurrentLine != '\0') { //! '\0' = ���ڿ��� ��
		int lineWidth = 0;

		// �ؽ�Ʈ Max ����
		while (lineWidth < maxWidth && pCurrentLine[lineWidth] != '\0' && pCurrentLine[lineWidth] != '\n') {
			lineWidth++;
		}

		RECT lineRect = rc;
		lineRect.right = rc.left + lineWidth;

		m_pFont->DrawTextW(m_pSprite, pCurrentLine, lineWidth, &lineRect, DT_NOCLIP, Color);

		rc.top += iLineHeight;
		rc.bottom += iLineHeight;

		// ���� �ٷ� �̵�
		if (pCurrentLine[lineWidth] == '\n') {
			pCurrentLine += lineWidth + 1;
		}
		else {
			pCurrentLine += lineWidth;
		}

	}
	m_pSprite->End();
}

CMyFont * CMyFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight)
{
	CMyFont *	pInstance = new CMyFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		Safe_Release(pInstance);
		MSG_BOX("��Ʈ ������");
		return nullptr;
	}

	return pInstance;
}

void CMyFont::Free()
{
}
