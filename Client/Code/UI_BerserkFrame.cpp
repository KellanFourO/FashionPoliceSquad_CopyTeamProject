#include "stdafx.h"
#include "UI_BerserkFrame.h"
#include "UIMgr.h"


#include "Export_Utility.h"
#include "Export_System.h"



CBerserkFrame_UI::CBerserkFrame_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CBerserkFrame_UI::CBerserkFrame_UI(const CBerserkFrame_UI& rhs)
	: Engine::CGameObject(rhs)
{
}

CBerserkFrame_UI::~CBerserkFrame_UI()
{
}

HRESULT Engine::CBerserkFrame_UI::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_vPos = { 400.f, 570.f, 0.f };
	m_vScale = { 15.f, 23.f, 1.f };

	m_fX = m_vPos.x - WINCX * 0.5f; // 150 - 400 = -250
	m_fY = -m_vPos.y + WINCY * 0.5f; // -50 + 300 = 250

	//m_tInfo.vPos = m_vPos;
	//m_tInfo.vSize = m_vScale;

	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	m_pPlayer = Management()->Get_Player();

	return S_OK;
}

Engine::_int Engine::CBerserkFrame_UI::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	m_pTransformCom->m_vScale.x = m_vScale.x;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_fY;

	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CBerserkFrame_UI::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBerserkFrame_UI::Render_GameObject()
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

HRESULT Engine::CBerserkFrame_UI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BerserkFrameTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CBerserkFrame_UI* CBerserkFrame_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBerserkFrame_UI* pInstance = new CBerserkFrame_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Berserk Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CBerserkFrame_UI::Free()
{
	__super::Free();
}
