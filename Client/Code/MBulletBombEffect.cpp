#include "stdafx.h"
#include "MBulletBombEffect.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CMBulletBombEffect::CMBulletBombEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CMBulletBombEffect::CMBulletBombEffect(const CMBulletBombEffect& rhs)
	: CGameObject(rhs)
{

}

CMBulletBombEffect::~CMBulletBombEffect()
{

}

HRESULT Engine::CMBulletBombEffect::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
	m_pTransformCom->Set_Scale(_vec3{ 6.f, 6.f, 6.f });

	m_pCollider->Set_Host(this);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	return S_OK;
}

Engine::_int Engine::CMBulletBombEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_pPlayerTransform = dynamic_cast<CTransform*>(Management()->Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));

	_vec3 vPlayerPos, vMyPos, vLook;	

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	vLook = vPlayerPos - vMyPos;
	D3DXVec3Normalize(&vLook, &vLook);

	_float fAngle = atan2f(vLook.x, vLook.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
	
	m_pCollider->SetCenterPos(m_pTransformCom->m_vInfo[INFO_POS]);
	m_fFrame +=87.f* fTimeDelta;

	if (87.f < m_fFrame)
		return OBJ_DEAD;

	_int iExit = __super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void Engine::CMBulletBombEffect::LateUpdate_GameObject()
{
	
}

void CMBulletBombEffect::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(10.f);

		//TODO 플레이어 총알 오브젝트 풀링 할거면 여기서
	}
	else
		return;
}

void CMBulletBombEffect::OnCollisionStay(CCollider* _pOther)
{
}

void CMBulletBombEffect::OnCollisionExit(CCollider* _pOther)
{
}

CMBulletBombEffect* CMBulletBombEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMBulletBombEffect* pInstance = new CMBulletBombEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MonsterBombEffect Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMBulletBombEffect::Render_GameObject()
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

void Engine::CMBulletBombEffect::Free()
{
	__super::Free();
}

HRESULT Engine::CMBulletBombEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pEffectBufferCom = dynamic_cast<CEffectTex*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MBulletBombEffectTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	/*pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", pComponent });*/

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}
