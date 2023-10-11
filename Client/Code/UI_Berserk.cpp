#include "stdafx.h"
#include "UI_Berserk.h"
#include "UIMgr.h"


#include "Export_Utility.h"
#include "Export_System.h"



CBerserk_UI::CBerserk_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CBerserk_UI::CBerserk_UI(const CBerserk_UI& rhs)
	: Engine::CGameObject(rhs)
{
}

CBerserk_UI::~CBerserk_UI()
{
}

HRESULT Engine::CBerserk_UI::Ready_GameObject()
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

Engine::_int Engine::CBerserk_UI::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	_float fMaxHp = m_pPlayer->Get_INFO()->fMaxHP;
	_float fCurHp = m_pPlayer->Get_INFO()->fHP;

	_float fRatio = fCurHp / fMaxHp;
	_float fOrigin = m_tInfo.vSize.x;
	_float fSecond = fOrigin * fRatio;

	_float fResult = fOrigin - fSecond;

	m_pBufferCom->SetRatio(fRatio);

	m_pTransformCom->m_vScale.x = m_vScale.x * fRatio;
	m_pTransformCom->m_vScale.y = m_vScale.y;
	m_fX = m_vPos.x - fResult;
	m_fY = m_vPos.y;
	m_pTransformCom->m_vInfo[INFO_POS].x = m_fX - WINCX * 0.5f;
	m_pTransformCom->m_vInfo[INFO_POS].y = -m_fY + WINCY * 0.5f;

	_int iExit = __super::Update_GameObject(fTimeDelta);


	return 0;
}

void Engine::CBerserk_UI::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBerserk_UI::Render_GameObject()
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

HRESULT Engine::CBerserk_UI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BerserkTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CBerserk_UI* CBerserk_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBerserk_UI* pInstance = new CBerserk_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Berserk Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CBerserk_UI::Free()
{
	__super::Free();
}
