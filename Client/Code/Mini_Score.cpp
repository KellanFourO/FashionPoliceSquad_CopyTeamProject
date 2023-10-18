
#include "stdafx.h"
#include "Mini_Score.h"

#include "Export_System.h"
#include "Export_Utility.h"


CMini_Score::CMini_Score(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMini_Score::CMini_Score(const CMini_Score& rhs)
	: Engine::CGameObject(rhs)
{
}

CMini_Score::~CMini_Score()
{
}

HRESULT CMini_Score::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mini_Number_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

void CMini_Score::Set_TexNum(float m_Tex)
{
	m_iTextureIndex = m_Tex;
}

HRESULT CMini_Score::Ready_GameObject(float pPos_X, int Count)
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_STDCount = Count;

	_vec3 vPos, vScale;
	_float fMultiply = 1.f;

	if (Count == 0)
	{
		vPos = { pPos_X - 5.f, 150.f, 0.f };
		vScale = { 20.f * fMultiply, 20.f * fMultiply, 1.f };
	}
	else {
		vPos = { pPos_X, 150.f, 0.f };
		vScale = { 15.f * fMultiply, 15.f * fMultiply, 1.f };
	}
	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_PlayerPos = vPos;
	m_PlayerScale = vScale;

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	// -1 ~ 1 -> 0 ~ 2

	return S_OK;
}

_int CMini_Score::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	int iExit = __super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMini_Score::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMini_Score::Render_GameObject()
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


CMini_Score* CMini_Score::Create(LPDIRECT3DDEVICE9 pGraphicDev, float pPos_X, int Count)
{
	CMini_Score* pInstance = new CMini_Score(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos_X, Count)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MINIGame_Player Create Fail");
		return nullptr;
	}

	return pInstance;
}

void CMini_Score::Free()
{
	__super::Free();
}
