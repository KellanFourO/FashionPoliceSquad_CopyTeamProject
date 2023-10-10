#include "stdafx.h"
#include "..\Header\PaintBulletTrace.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CPaintBulletTrace::CPaintBulletTrace(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CPaintBulletTrace::CPaintBulletTrace(const CPaintBulletTrace& rhs)
	: CGameObject(rhs)
{

}

CPaintBulletTrace::~CPaintBulletTrace()
{

}

HRESULT Engine::CPaintBulletTrace::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::EFFECT;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	//m_pTransformCom->Set_Pos(_vec3(_float(rand() % 20), 10.f, _float(rand() % 20)));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS,&vPos);

	return S_OK;
}

Engine::_int Engine::CPaintBulletTrace::Update_GameObject(const _float& fTimeDelta)
{

	m_PaintBulletTraceDeadTime += fTimeDelta;

// 	m_fFrame += 87.f * fTimeDelta;
// 
// 	if (87.f < m_fFrame)
// 		m_fFrame = 0.f;
// 
// 	
// 
// 	_matrix		matWorld, matView, matBill;
// 
// 	matWorld = *m_pTransformCom->Get_WorldMatrix();
// 
// 	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
// 	D3DXMatrixIdentity(&matBill);
// 
// 	matBill._11 = matView._11;
// 	matBill._13 = matView._13;
// 	matBill._31 = matView._31;
// 	matBill._33 = matView._33;
// 
// 	D3DXMatrixInverse(&matBill, 0, &matBill);
// 
// 	// 주의 사항
// 
// 	// 빌(자전의 역행렬) * 월드(스 * 자 * 이)
// 
// 	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	


	//++m_PaintBulletTraceDeadTime;		//Update에서 탄흔 죽여버릴거임 프레임 단위로 컷트놓을 예정
	if (m_PaintBulletTraceDeadTime > 3.f)
		return OBJ_DEAD;
	
	
	_int iExit = __super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void Engine::CPaintBulletTrace::LateUpdate_GameObject()
{
	
	//_vec3	vPos;
	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//
	//__super::Compute_ViewZ(&vPos);
}

CPaintBulletTrace* CPaintBulletTrace::Create(LPDIRECT3DDEVICE9 pGraphicDev, COLORTAG pColorTag)
{
	CPaintBulletTrace* pInstance = new CPaintBulletTrace(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("PaintBulletTrace Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPaintBulletTrace::Render_GameObject()
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

void Engine::CPaintBulletTrace::Free()
{
	__super::Free();
}

HRESULT Engine::CPaintBulletTrace::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pEffectBufferCom = dynamic_cast<CEffectTex*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

// 	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EffectTexture"));
// 	NULL_CHECK_RETURN(pComponent, E_FAIL);
// 	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PaintBulletTrace"));
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
