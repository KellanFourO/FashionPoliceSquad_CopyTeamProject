#include "stdafx.h"
#include "UI_HPBarValue.h"

#include "Export_Utility.h"
#include "Export_System.h"


CHPBarValue::CHPBarValue(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CHPBarValue::CHPBarValue(const CHPBarValue& rhs)
	: Engine::CGameObject(rhs)
{
}

CHPBarValue::~CHPBarValue()
{
}

HRESULT Engine::CHPBarValue::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPos = { 210.f, 580.f, 0.1f };
	m_vScale = { 59.f, 4.f, 0.f };

	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);

	m_pPlayer = Management()->Get_Player();

	return S_OK;
}

Engine::_int Engine::CHPBarValue::Update_GameObject(const _float& fTimeDelta)
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

void Engine::CHPBarValue::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CHPBarValue::Render_GameObject()
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

HRESULT Engine::CHPBarValue::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPValueTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CHPBarValue* CHPBarValue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHPBarValue* pInstance = new CHPBarValue(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("HPBar Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CHPBarValue::Free()
{
	__super::Free();
}
