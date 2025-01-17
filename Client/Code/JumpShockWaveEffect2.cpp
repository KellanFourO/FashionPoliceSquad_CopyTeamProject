#include "stdafx.h"
#include "JumpShockWaveEffect2.h"

#include "Export_Utility.h"
#include "EffectTex.h"

CJumpShockWaveEffect2::CJumpShockWaveEffect2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CJumpShockWaveEffect2::CJumpShockWaveEffect2(const CJumpShockWaveEffect2& rhs)
	: CGameObject(rhs)
{

}

CJumpShockWaveEffect2::~CJumpShockWaveEffect2()
{

}

HRESULT Engine::CJumpShockWaveEffect2::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);

	_vec3 vPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).back()->m_pTransformCom->m_vInfo[INFO_POS];
	m_fSpeed = 0.5f;
	m_pPlayerTransform = Management()->Get_Player()->Get_Transform();
	
	m_pTransformCom->Set_Scale(_vec3{ 2.f, 2.f, 2.f });
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Host(this);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	return S_OK;
}

Engine::_int Engine::CJumpShockWaveEffect2::Update_GameObject(const _float& fTimeDelta)
{
	//m_pCollider->SetCenterPos(m_pTransformCom->m_vInfo[INFO_POS]);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

	m_fFrame += 77.f * fTimeDelta;
	m_fEffectDieTime += fTimeDelta;

	float fRadian;

	fRadian = 3.14 * m_fAngle / 180.f;

	m_pTransformCom->m_vInfo[INFO_POS].x += m_fSpeed * cosf(fRadian);
	m_pTransformCom->m_vInfo[INFO_POS].z -= m_fSpeed * sinf(fRadian);

	if (77.f <= m_fFrame)
		m_fFrame = 77.f;
	m_pTransformCom->Set_Scale(*m_pTransformCom->Get_Scale()*1.02f);
	if (m_fEffectDieTime > 2.f)
		return OBJ_DEAD;



	_vec3 vPlayerPos, vMyPos, vLook;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	vLook = vPlayerPos - vMyPos;
	D3DXVec3Normalize(&vLook, &vLook);



	_float fAngle = atan2f(vLook.x, vLook.z);
	m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);


	_int iExit = __super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return iExit;
}

void Engine::CJumpShockWaveEffect2::LateUpdate_GameObject()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
	__super::LateUpdate_GameObject();
}

CJumpShockWaveEffect2* CJumpShockWaveEffect2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos)
{
	CJumpShockWaveEffect2* pInstance = new CJumpShockWaveEffect2(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("JumpShockWaveEffect2 Create Failed");
		return nullptr;
	}

	pInstance->Get_Transform()->Set_Pos(vCreatePos);

	return pInstance;
}

void CJumpShockWaveEffect2::Render_GameObject()
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

void CJumpShockWaveEffect2::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(m_fDMG);
		
		//TODO 플레이어 총알 오브젝트 풀링 할거면 여기서
	}
	else
		return;
}

void CJumpShockWaveEffect2::OnCollisionStay(CCollider* _pOther)
{
	
// 	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
// 	{
// 		dynamic_cast<CPlayer*>(_pOther->Get_Host())->Attacked(m_fDMG);
// 		//TODO 플레이어 총알 오브젝트 풀링 할거면 여기서
// 	}
// 	else
// 		return;
}

void CJumpShockWaveEffect2::OnCollisionExit(CCollider* _pOther)
{
}


void Engine::CJumpShockWaveEffect2::Free()
{
	__super::Free();
}

HRESULT Engine::CJumpShockWaveEffect2::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pEffectBufferCom = dynamic_cast<CEffectTex*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_JumpShockWaveEffectTexture2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}
