#include "stdafx.h"
#include "UI_MyCardTexture.h"
#include "UIMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include <random>

CMyCardTexture::CMyCardTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CMyCardTexture::CMyCardTexture(const CMyCardTexture& rhs)
	: Engine::CGameObject(rhs)
{
}

CMyCardTexture::~CMyCardTexture()
{
}

HRESULT Engine::CMyCardTexture::Ready_GameObject(_float StartX)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 200.f;
	m_fHorMultyply = 0.119f;
	m_fVerMultyply = 0.185f;
	m_vPos = { StartX, -50.f, 50.f };
	m_vScale = { 512.f * m_fHorMultyply, 512.f * m_fVerMultyply, 1.f };

	m_vPos.x = m_vPos.x - WINCX * 0.5f;
	m_vPos.y = -m_vPos.y + WINCY * 0.5f;


	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(m_vPos);


	return S_OK;
}

Engine::_int Engine::CMyCardTexture::Update_GameObject(const _float& fTimeDelta, _vec3& vPos, _float fAngle)
{

	Engine::Add_RenderGroup(RENDER_UI, this);



	_vec3 vScale = *m_pTransformCom->Get_Scale();
	vScale.x = -abs(vScale.x);

	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(fAngle));

	_int iExit = __super::Update_GameObject(fTimeDelta);
	return 0;
}

void Engine::CMyCardTexture::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMyCardTexture::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCardCom->Render_Textrue();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Engine::CMyCardTexture::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CUITex*>(Engine::Clone_Proto(L"Proto_UITex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCardCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CardBackTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);
	return S_OK;
}

CMyCardTexture* CMyCardTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float StartY)
{
	CMyCardTexture* pInstance = new CMyCardTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(StartY)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyCardTexture Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Engine::CMyCardTexture::Free()
{

	__super::Free();
}
