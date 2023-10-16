#include "stdafx.h"
#include "UI_BossHPValue.h"

#include "Export_Utility.h"
#include "Export_System.h"


CBossHPValue::CBossHPValue(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(UIDATA));
}

CBossHPValue::CBossHPValue(const CBossHPValue& rhs)
	: Engine::CGameObject(rhs)
{
}

CBossHPValue::~CBossHPValue()
{
}

HRESULT Engine::CBossHPValue::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fRatio = 1.f;


	m_vPos = { 420, 30, 0.1f };
	m_vScale = { 250 * fRatio, 10.f * fRatio, 0.f };

	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);



	return S_OK;
}

Engine::_int Engine::CBossHPValue::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	if (m_bLateInit)
	{
		m_pBoss = dynamic_cast<CStage1Boss*>(Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::BOSS).back());

		m_bLateInit = true;
	}

	if(m_pBoss->Get_Info().bDead)
		return OBJ_DEAD;


	_float fMaxHp = m_pBoss->Get_Info().fMaxHP;
	_float fCurHp = m_pBoss->Get_Info().fHP;

	_float fRatio = fCurHp / fMaxHp;
	_float fOrigin = m_vScale.x;
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

void Engine::CBossHPValue::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBossHPValue::Render_GameObject()
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

HRESULT Engine::CBossHPValue::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHPBarTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);


	return S_OK;
}


CBossHPValue* CBossHPValue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossHPValue* pInstance = new CBossHPValue(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("HPBar Create Failed");
		return nullptr;
	}
	return pInstance;
}

void Engine::CBossHPValue::Free()
{
	__super::Free();
}
