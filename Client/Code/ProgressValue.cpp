#include "stdafx.h"
#include "ProgressValue.h"

#include "Export_System.h"
#include "Export_Utility.h"

CProgressValue::CProgressValue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CProgressValue::CProgressValue(const CProgressValue& rhs)
	: Engine::CGameObject(rhs)
{
}

CProgressValue::~CProgressValue()
{
}

HRESULT CProgressValue::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ProgressBarValueTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);



	return S_OK;
}


CProgressValue* CProgressValue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProgressValue* pInstance = new CProgressValue(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("ProgressValue Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProgressValue::Free()
{
	__super::Free();
}



HRESULT CProgressValue::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_ViewMatrix);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 100.f);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fXRatio = 6.f;
	_float fYRatio = 2.f;

	m_fSizeX = 128.0f * fXRatio;
	m_fSizeY = 16.0f * fYRatio;

	m_fX = 400.f;
	m_fY = 580.f;

	_vec3 vPos, vScale;

	vScale = { m_fSizeX * 0.5f, m_fSizeY * 0.5f, 1.f };

	vPos = { m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f };

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

Engine::_int CProgressValue::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);



	return 0;
}

void CProgressValue::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}
void CProgressValue::Render_GameObject()
{
	_float fMaxProgress = 100; // 431
	_float fCurProgress = m_fValueRatio;

	_float fRatio = fCurProgress / fMaxProgress;
	_float fOrigin = m_fSizeX * 0.5f;
	_float fSecond = fOrigin * fRatio;

	_float fResult = fOrigin - fSecond;
	m_pBufferCom->SetRatio(fRatio);

	_vec3 vPos, vScale;



	vScale = { (m_fSizeX * 0.5f) * fRatio, m_fSizeY * 0.5f, 1.f };
	vPos = { (m_fX - WINCX * 0.5f) - fResult, -m_fY + WINCY * 0.5f, 0.f };

	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Scale(vScale);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pTextureCom->Render_Textrue(0);
	m_pBufferCom->Render_Buffer();


	if (m_fValueRatio != 100)
		Engine::Render_Font(L"DIALOG_FONT", L"로딩 중...", &_vec2(400, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)));
	else
		Engine::Render_Font(L"DIALOG_FONT", L"시작하려면 엔터키를 눌러주세요.", &_vec2(150, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)));

	wstring wstrTemp = to_wstring((int)m_fValueRatio).c_str();
	wstrTemp = wstrTemp + L"\%";

	Engine::Render_Font(L"DIALOG_FONT", wstrTemp.c_str(), &_vec2(700, 550), D3DXCOLOR(D3DCOLOR_ARGB(255, 130, 245, 209)));


}