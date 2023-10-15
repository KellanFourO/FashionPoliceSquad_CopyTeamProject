#include "stdafx.h"
#include "JumpShockWaveEffect.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CJumpShockWaveEffect::CJumpShockWaveEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CJumpShockWaveEffect::CJumpShockWaveEffect(const CJumpShockWaveEffect& rhs)
	: CGameObject(rhs)
{

}

CJumpShockWaveEffect::~CJumpShockWaveEffect()
{

}

HRESULT Engine::CJumpShockWaveEffect::Ready_GameObject()
{
	m_eObjectTag = OBJECTTAG::EFFECT;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3{ 25.f, 25.f, 25.f });

	m_pPlayerTransform = Management()->Get_Player()->Get_Transform();


	return S_OK;
}

Engine::_int Engine::CJumpShockWaveEffect::Update_GameObject(const _float& fTimeDelta)
{



	m_fFrame += 9.f * fTimeDelta;

	if (9.f < m_fFrame)
		return OBJ_DEAD;

	_vec3 vPlayerPos, vMyPos, vLook;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	vLook = vPlayerPos - vMyPos;
	D3DXVec3Normalize(&vLook, &vLook);



	_float fAngle = atan2f(vLook.x, vLook.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);


	_int iExit = __super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void Engine::CJumpShockWaveEffect::LateUpdate_GameObject()
{

}

CJumpShockWaveEffect* CJumpShockWaveEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos)
{
	CJumpShockWaveEffect* pInstance = new CJumpShockWaveEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("JumpShockWaveEffect Create Failed");
		return nullptr;
	}

	pInstance->Get_Transform()->Set_Pos(vCreatePos);

	return pInstance;
}

void CJumpShockWaveEffect::Render_GameObject()
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

void Engine::CJumpShockWaveEffect::Free()
{
	__super::Free();
}

HRESULT Engine::CJumpShockWaveEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pEffectBufferCom = dynamic_cast<CEffectTex*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_JumpShockWaveEffectTexture"));
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
