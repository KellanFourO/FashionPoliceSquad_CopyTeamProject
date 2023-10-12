#include "stdafx.h"
#include "UI_HPBarFrame.h"

#include "Export_Utility.h"
#include "Export_System.h"



CHPBarFrame::CHPBarFrame(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CHPBarFrame::CHPBarFrame(const CHPBarFrame& rhs)
	: Engine::CGameObject(rhs)
{
}

CHPBarFrame::~CHPBarFrame()
{
}

HRESULT Engine::CHPBarFrame::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_vPos = { 220.f, 580.f, 1.f };
	m_vScale = { 54.f, 8.f, 0.f };

	m_fX = m_vPos.x - WINCX * 0.5f; // 150 - 400 = -250
	m_fY = -m_vPos.y + WINCY * 0.5f; // -50 + 300 = 250

	m_pTransformCom->m_vScale.x = m_vScale.x;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_fY;

	m_pPlayer = Management()->Get_Player();

	return S_OK;
}

Engine::_int Engine::CHPBarFrame::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CHPBarFrame::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CHPBarFrame::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);


	m_pTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CHPBarFrame::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPFrameTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CHPBarFrame* CHPBarFrame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHPBarFrame* pInstance = new CHPBarFrame(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("HPBar Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CHPBarFrame::Free()
{
	__super::Free();
}
