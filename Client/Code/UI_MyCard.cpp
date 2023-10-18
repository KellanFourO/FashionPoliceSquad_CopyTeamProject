#include "stdafx.h"
#include "UI_MyCard.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include <random>
#include "Player.h"

CMyCard::CMyCard(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev),
	m_bface(false),
	m_bStart(false),
	m_bPicking(false),
	m_fSpeed(0),
	m_fMultiply(0),
	m_fMaxMoveY(0),
	m_fAngle(0)
{
}

CMyCard::CMyCard(const CMyCard& rhs)
	: Engine::CGameObject(rhs)
{
}

CMyCard::~CMyCard()
{
}

HRESULT Engine::CMyCard::Ready_GameObject(_float StartX)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCardTexture = CMyCardTexture::Create(m_pGraphicDev, StartX);

	m_fSpeed = 200.f;
	m_fMultiply = 0.3f;
	m_fMaxMoveY = 0.f;
	m_fMinMoveY = -200.f;

	m_bface = false;  // 카드 앞면 뒷면
	m_bStart = true; // 레벨업 시에만 부를 예정
	m_bPicking = false;

	m_vPos = { StartX, 0.f, 50.f };
	m_vScale = { 256.f * m_fMultiply, 512.f * m_fMultiply, 1.f };

	//m_tRect = { (_long)m_vScale.x - (m_vScale.x * 0.5f), }
	//m_tRect.left = (_long)m_vScale.x - m_vScale.x * 0.5f;
	//m_tRect =
	//{
	//	(LONG)(m_vPos.x - m_vScale.x), // 왼쪽 X 좌표
	//	(LONG)(m_vPos.y - m_vScale.y), // 위쪽 Y 좌표
	//	(LONG)(m_vPos.x + m_vScale.x), // 오른쪽 X 좌표
	//	(LONG)(m_vPos.y + 28 + m_vScale.y)  // 아래쪽 Y 좌표
	//};


	m_vPos.x = m_vPos.x - WINCX * 0.5f;
	m_vPos.y = -m_vPos.y + WINCY * 0.5f;


	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardBlueEyeDragon.png", 1); // 대쉬
	m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardFedderMan.png", 1);// 로프 액션
	m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardYugioh.png", 1); // 기타
	m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardOvelisk.png", 1); // 기타
	m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardBonus.png", 1); // 기타
	m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardGreedPot.png", 1); // 기타

	/*RandomCard();*/
	SelectTexture();

	return S_OK;
}

Engine::_int Engine::CMyCard::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);

	CardAnimation(fTimeDelta);

	if(!m_bStart)
	{
		CardPicking(fTimeDelta);
	}
	_int iExit = __super::Update_GameObject(fTimeDelta);
	m_fTimeDelta = fTimeDelta;

	return 0;
}

void Engine::CMyCard::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	m_pCardTexture->LateUpdate_GameObject();
}

void CMyCard::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	if (m_bface)
	{
		m_pTextureFrontCom->Render_Textrue();
		m_pBufferCom->Render_Buffer();

		_vec3 vCardPos = m_vPos;
		vCardPos.y = vCardPos.y + 33.f;

		m_pCardTexture->Update_GameObject(m_fTimeDelta, vCardPos, m_fAngle);
	}
	else
	{
		m_pTextureBackCom->Render_Textrue();
		m_pBufferCom->Render_Buffer();
	}


	//// 빨간색 선을 그리기 위한 D3DXLine 인터페이스 생성
	//LPD3DXLINE pLine;
	//D3DXCreateLine(m_pGraphicDev, &pLine);

	//// 빨간색 선의 색상 설정 (D3DCOLOR_XRGB 함수를 사용하여 색상을 지정)
	//D3DCOLOR redColor = D3DCOLOR_XRGB(255, 0, 0);

	//// 선의 시작점과 끝점 설정 (m_tRect의 네 점을 연결하여 사각형 경계를 그립니다)
	//D3DXVECTOR2 points[5] =
	//{
	//	D3DXVECTOR2(static_cast<FLOAT>(m_tRect.left), static_cast<FLOAT>(m_tRect.top)),
	//	D3DXVECTOR2(static_cast<FLOAT>(m_tRect.right), static_cast<FLOAT>(m_tRect.top)),
	//	D3DXVECTOR2(static_cast<FLOAT>(m_tRect.right), static_cast<FLOAT>(m_tRect.bottom)),
	//	D3DXVECTOR2(static_cast<FLOAT>(m_tRect.left), static_cast<FLOAT>(m_tRect.bottom)),
	//	D3DXVECTOR2(static_cast<FLOAT>(m_tRect.left), static_cast<FLOAT>(m_tRect.top)) // 마지막 점은 시작점과 같습니다.
	//};

	//// 선 그리기
	//pLine->Begin();
	//pLine->Draw(points, 5, redColor);
	//pLine->End();

	//// ... 나머지 렌더링 코드 ...

	//// D3DXLine 인터페이스 해제
	//Safe_Release(pLine);

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CMyCard::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureFrontCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CardFrontTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pTextureBackCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CardBackTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMyCard::CardAnimation(const _float& fTimeDelta)
{

	if (m_vPos.y >= m_fMaxMoveY & !m_bPicking)
	{
		m_vPos.y -= m_fSpeed * fTimeDelta;

		m_pTransformCom->Set_Pos(m_vPos);

		//m_tRect.top += m_fSpeed * fTimeDelta;
		//m_tRect.bottom += m_fSpeed * fTimeDelta;
	}
	else
	{
		m_bStart = false;
	}


}

_bool CMyCard::CardOpen(const _float& fTimeDelta)
{
	if (!m_bface)
	{
		if (m_fAngle < 180.f)
		{
			m_fAngle += 60.f * fTimeDelta;
			if (m_fAngle > 90.f)
			{
				m_bface = true;
				//m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(180.f));

			}
			else
			{
				m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_fAngle));
			}

		}
	}
	else
	{
		if (m_fAngle < 180.f)
		{
			m_fAngle += 140.f * fTimeDelta;
			m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_fAngle));
		}
		else
		{
			m_fAngle = 180.f;
			m_bStart = false;
			return true;
		}

	}

	return false;
}

void CMyCard::CardPicking(const _float& fTimeDelta)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	//if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80 && m_bJump == false)
	//{
	//
	//}
	//if (PtInRect(&m_tRect, ptMouse))
	//{
	//	if (Engine::Get_DIMouseState(DIM_LB))
	//	{
	//		MSG_BOX("픽킹");
	//		m_bPicking = true;
	//	}
	//}

	if (m_bPicking)
	{
		if (CardOpen(fTimeDelta))
		{
			if (m_vPos.y >= m_fMinMoveY)
			{
				m_vPos.y += m_fSpeed * fTimeDelta;

				m_pTransformCom->Set_Pos(m_vPos);

				if (m_bRealPick)
				{
					CPlayer* pPlayer = Management()->Get_Player();

					switch (m_eCardType)
					{
					case Engine::CARD_TYPE::FORCE:
						pPlayer->DashOn();
						m_bRealPick = false;
						break;
					case Engine::CARD_TYPE::SPEED:
						pPlayer->RopeOn();
						m_bRealPick = false;
						break;
					case Engine::CARD_TYPE::INTELLIGENCE:
						pPlayer->Get_INFO()->fMoveSpeed = pPlayer->Get_INFO()->fMoveSpeed + 1.f;
						m_bRealPick = false;
						break;
					case Engine::CARD_TYPE::STRENGTH:
						pPlayer->Get_INFO()->fMaxHP = pPlayer->Get_INFO()->fMaxHP + 10.f;
						pPlayer->Get_INFO()->fMaXHP_Additional = pPlayer->Get_INFO()->fMaXHP_Additional + 5.f;
						m_bRealPick = false;
						break;
					case Engine::CARD_TYPE::BONUS:
						break;
					case Engine::CARD_TYPE::BOMB:
						break;
					case Engine::CARD_TYPE::CARD_TYPE_END:
						break;
					default:
						break;
					}
				}

				//m_tRect.top -= m_fSpeed * fTimeDelta;
				//m_tRect.bottom -= m_fSpeed * fTimeDelta;
			}
		}
		//TODO - 승용 . 여기서 능력치 추가 한후 사라져야함.
	}
}

// void CMyCard::RandomCard()
// {
// 	random_device rd;
// 	mt19937 gen(rd());
//
// 	uniform_int_distribution<int> distribution(0, 5); // 랜덤 시작 부터 마지막
//
// 	int iRandomValue = distribution(gen);
//
// 	m_eCardType = (CARD_TYPE)iRandomValue;
//
// 	SelectTexture();
// }

void CMyCard::SelectTexture()
{

	//m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardBlueEyeDragon.png", 1); // 대쉬
	//m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardFedderMan.png", 1);// 로프 액션
	//m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardYugioh.png", 1); // 기타
	//m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardOvelisk.png", 1); // 기타
	//m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardBonus.png", 1); // 기타
	//m_pTextureFrontCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Card/CardGreedPot.png", 1); // 기타

	switch (m_eCardType)
	{
	case CARD_TYPE::FORCE:
		m_pCardTexture->Get_Texture()->Set_Texture(m_pTextureFrontCom->Get_Texture(1), 0);
		break;
	case CARD_TYPE::SPEED:
		m_pCardTexture->Get_Texture()->Set_Texture(m_pTextureFrontCom->Get_Texture(2), 0);
		break;
	case CARD_TYPE::INTELLIGENCE:
		m_pCardTexture->Get_Texture()->Set_Texture(m_pTextureFrontCom->Get_Texture(3), 0);
		break;
	case CARD_TYPE::STRENGTH:
		m_pCardTexture->Get_Texture()->Set_Texture(m_pTextureFrontCom->Get_Texture(4), 0);
		break;
	case CARD_TYPE::BONUS:
		m_pCardTexture->Get_Texture()->Set_Texture(m_pTextureFrontCom->Get_Texture(5), 0);
		break;
	case CARD_TYPE::BOMB:
		m_pCardTexture->Get_Texture()->Set_Texture(m_pTextureFrontCom->Get_Texture(6), 0);
		break;
	}
}



CMyCard* CMyCard::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float StartX, CARD_DIR Dir)
{
	CMyCard* pInstance = new CMyCard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(StartX)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyCard Create Failed");
		return nullptr;
	}

	pInstance->m_eCardDir = Dir;
	return pInstance;
}

void Engine::CMyCard::Free()
{

	__super::Free();
	Safe_Release(m_pCardTexture);
}
