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



	return S_OK;
}

Engine::_int Engine::CMonsterBombEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_pPlayerTransform = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	_vec3 vPlayerPos, vMyPos, vLook;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	vLook = vPlayerPos - vMyPos;
	D3DXVec3Normalize(&vLook, &vLook);

	_float fAngle = atan2f(vLook.x, vLook.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
	

	m_fFrame +=15.f* fTimeDelta;

	if (15.f < m_fFrame)
	{
		m_fFrame = 0.f;
		++m_fLoop;
	}
	if(m_fLoop>2.f)
		return OBJ_DEAD;

	_int iExit = __super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void Engine::CMonsterBombEffect::LateUpdate_GameObject()
{
	
}

CMonsterBombEffect* CMonsterBombEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterBombEffect* pInstance = new CMonsterBombEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MonsterBombEffect Create Failed");
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MonsterBombEffectTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}
