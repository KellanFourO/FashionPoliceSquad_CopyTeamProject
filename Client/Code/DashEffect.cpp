#include "stdafx.h"
#include "DashEffect.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CDashEffect::CDashEffect(LPDIRECT3DDEVICE9 pGarphicDev)
	:CGameObject(pGarphicDev)
{
}

CDashEffect::CDashEffect(const CDashEffect& rhs)
	:CGameObject(rhs)
{
}

CDashEffect::~CDashEffect()
{
}

HRESULT CDashEffect::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_ObjectTag(OBJECTTAG::EFFECT);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.0f, 100.0f);

	_vec3 vPos, vScale;
	vScale = { 400.f, 300.f , 1.f };
	vPos = { 400.f, 300.f, 0.f };
	vPos.x = vPos.x - WINCX * 0.5f;
	vPos.y = -vPos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Scale(vScale);
	m_pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	return S_OK;
}

_int CDashEffect::Update_GameObject(const _float& fTimeDelta)
{
	++m_DashEffectDieTime;
	//_vec3 Temp = { 0.f,0.f,1.f };
	//m_pTransformCom->Set_Pos(m_pPlayerTransform->m_vInfo[INFO_POS] + Temp);
	if (m_DashEffectDieTime>10.f)
		return OBJ_DEAD;

	_int iExit = __super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return iExit;
}

void CDashEffect::LateUpdate_GameObject()
{
}

void CDashEffect::Render_GameObject()
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
	m_pEffectBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CDashEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pEffectBufferCom = dynamic_cast<CEffectTex*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DashEffectTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

CDashEffect* CDashEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDashEffect* pInstance = new CDashEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("DashEffect Create Failed");

		return nullptr;
	}
	return pInstance;
}

void CDashEffect::Free()
{
	__super::Free();
}
