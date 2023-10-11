#include "stdafx.h"
#include "..\Header\MonsterBombEffect.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CMonsterBombEffect::CMonsterBombEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CMonsterBombEffect::CMonsterBombEffect(const CMonsterBombEffect& rhs)
	: CGameObject(rhs)
{

}

CMonsterBombEffect::~CMonsterBombEffect()
{

}

HRESULT Engine::CMonsterBombEffect::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::EFFECT;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3{ 6.f, 6.f, 6.f });
	//m_pTransformCom->Set_Pos(_vec3(_float(rand() % 20), 10.f, _float(rand() % 20)));
	m_pTransformCom->Set_Pos(_vec3{ 40.f,5.f,20.f });
	return S_OK;
}

Engine::_int Engine::CMonsterBombEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 87.f * fTimeDelta;

	if (87.f < m_fFrame)
		m_fFrame = 0.f;

	_int iExit = __super::Update_GameObject(fTimeDelta);

	_matrix		matWorld, matView, matBill;

	matWorld = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 주의 사항

	// 빌(자전의 역행렬) * 월드(스 * 자 * 이)

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));


	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void Engine::CMonsterBombEffect::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

CMonsterBombEffect* CMonsterBombEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterBombEffect* pInstance = new CMonsterBombEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMonsterBombEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue(_ulong(m_fFrame));
	m_pEffectBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Engine::CMonsterBombEffect::Free()
{
	__super::Free();
}

HRESULT Engine::CMonsterBombEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pEffectBufferCom = dynamic_cast<CEffectTex*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EffectTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	/*pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", pComponent });*/

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}
