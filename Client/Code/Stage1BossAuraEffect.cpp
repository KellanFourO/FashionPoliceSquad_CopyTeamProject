#include "stdafx.h"
#include "Stage1BossAuraEffect.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CStage1BossAuraEffect::CStage1BossAuraEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CStage1BossAuraEffect::CStage1BossAuraEffect(const CStage1BossAuraEffect& rhs)
	: CGameObject(rhs)
{

}

CStage1BossAuraEffect::~CStage1BossAuraEffect()
{

}

HRESULT Engine::CStage1BossAuraEffect::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::EFFECT;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3{ 40.f, 40.f, 40.f });
	m_pBufferCom->SetCount(4,1);
	m_pPlayerTransform = Management()->Get_Player()->Get_Transform();


	return S_OK;
}

Engine::_int Engine::CStage1BossAuraEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pHost->Get_Dead())
		return OBJ_DEAD;

	m_fFrame += fTimeDelta;

	if (m_fFrame > 0.08f)
	{
		++m_iTextureIndex;
		m_fFrame = 0.f;

		if (m_iTextureIndex > 4)
			m_iTextureIndex = 1;
	}



	_vec3 vPlayerPos, vMyPos, vLook;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	vLook = vPlayerPos - vMyPos;
	D3DXVec3Normalize(&vLook, &vLook);
	_float fAngle = atan2f(vLook.x, vLook.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);

	_vec3 vCenterPos, vHostPos;

	m_pHost->Get_Transform()->Get_Info(INFO_POS,&vHostPos);

	vCenterPos = vHostPos;
	vCenterPos.y += 12.f;
	m_pTransformCom->Set_Pos(vCenterPos);





	_int iExit = __super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return iExit;
}

void Engine::CStage1BossAuraEffect::LateUpdate_GameObject()
{

}

CStage1BossAuraEffect* CStage1BossAuraEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pHost)
{
	CStage1BossAuraEffect* pInstance = new CStage1BossAuraEffect(pGraphicDev);
	pInstance->m_pHost = pHost;

	if(!pInstance->m_pHost)
		return nullptr;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage1BossAuraEffect Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStage1BossAuraEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->Render_Textrue();
	m_pBufferCom->Render_Buffer(m_iTextureIndex,1);

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Engine::CStage1BossAuraEffect::Free()
{
	__super::Free();
}

HRESULT Engine::CStage1BossAuraEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Stage1Aura"));
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
