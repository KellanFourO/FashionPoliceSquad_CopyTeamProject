
#include "stdafx.h"
#include "Mini_Arrow.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_Arrow::CMini_Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_Arrow::CMini_Arrow(const CMini_Arrow& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_Arrow::~CMini_Arrow()
{
}



HRESULT CMini_Arrow::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Arrow_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMini_Arrow::Set_Rand_ArrowType(int iRandomNumber)
{
	m_iTextureIndex = iRandomNumber;

	switch (iRandomNumber)
	{
	case 0:
		m_eArrowType = MINIGAME_ARROW_TYPE::UP;
		break;

	case 1:
		m_eArrowType = MINIGAME_ARROW_TYPE::DOWN;
		break;

	case 2:
		m_eArrowType = MINIGAME_ARROW_TYPE::LEFT;
		break;

	case 3:
		m_eArrowType = MINIGAME_ARROW_TYPE::RIGHT;
		break;

	default:
		break;
	}
}

HRESULT CMini_Arrow::Ready_GameObject(int iRandomNumber)
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Rand_ArrowType(iRandomNumber);

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	_float StdSize = 20.f;
	_float StdDistance = 20.f;

	if (m_iArrowIndex < 10) { //0~9
		vPos = { 200.f + (m_iArrowIndex * (StdSize+ StdDistance)), 120.f, 0.f };
	}
	else if ((m_iArrowIndex >= 10) && (m_iArrowIndex < 20)) { //10~19
		vPos = { 600.f, 120.f + ((m_iArrowIndex - 10) * (StdSize + StdDistance)), 0.f };
	}
	else if ((m_iArrowIndex >= 20) && (m_iArrowIndex < 30)) { //20~29
		vPos = { 600.f - ((m_iArrowIndex - (20 - 1)) * (StdSize + StdDistance)), 480.f, 0.f };
	}
	vScale = { StdSize * fMultiply, StdSize * fMultiply, 1.f };

	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}




_int CMini_Arrow::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMini_Arrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMini_Arrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}



CMini_Arrow* CMini_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex, _int iRandomNumber)
{
	CMini_Arrow* pInstance = new CMini_Arrow(pGraphicDev);
	pInstance->Set_ArrowIndex(iIndex);


	if (FAILED(pInstance->Ready_GameObject(iRandomNumber)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Player Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_Arrow::Free()
{
	__super::Free();
}
