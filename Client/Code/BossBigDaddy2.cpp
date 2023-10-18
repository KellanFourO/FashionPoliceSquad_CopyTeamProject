#include "stdafx.h"
#include "BossBigDaddy2.h"

#include "BossBigDaddy_IDLE.h"
#include "BossBigDaddy_Dead.h"
#include "BossBigDaddy_Attack.h"
#include "DustGrey.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "MonsterBombEffect.h"
#include "MonsterState.h"
#include "Stage1Boss_BrifBigShield.h"
CBossBigDaddy2::CBossBigDaddy2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CBossBigDaddy2::CBossBigDaddy2(CMonster& rhs)
	: CMonster(rhs)
{
}

CBossBigDaddy2::~CBossBigDaddy2()
{
}

HRESULT CBossBigDaddy2::Ready_GameObject()
{
	__super::Ready_GameObject();

	Set_ObjectTag(OBJECTTAG::MONSTER);
	INFO.iMobType = MonsterType::BIGDADDY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	ReadyState();
	m_iMonsterIndex = 1;
	INFO.MonsterState = m_pStateArray[IDLE];
	INFO.MonsterState->Initialize(this);
	INFO.fHP = 100.f;
	INFO.fMaxHP = 100.f;


	m_fDectedRange = 150.f; //! 탐색 범위
	m_fAttackRange = 100.f; //! 공격 범위

	m_pTransformCom->Set_Scale({ 6.0f,12.0f,10.0f });

	//Set_Pos((_vec3{ 120.f,4.f,28.f }));
	//m_pTransformCom->Set_Pos((_vec3{ 198.5f,10.0f,235.5f }));

	m_pBufferCom->SetCount(4, 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/loose-suit-bossHit.png", 1);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	
	_vec3 vPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).back()->m_pTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->Set_Pos(vPos);

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());
	m_eHitType = BULLETTYPE::ASSERTRIFLE_BULLET;
	m_pMonsterBullet = nullptr;


	return S_OK;
}

_int CBossBigDaddy2::Update_GameObject(const _float& fTimeDelta)
{
	_vec3 vBossPos = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).back()->m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vBossLook = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).back()->m_pTransformCom->m_vInfo[INFO_LOOK];
	_vec3 vBossUp = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).back()->m_pTransformCom->m_vInfo[INFO_UP];

	D3DXVec3Normalize(&vBossLook, &vBossLook);
	_vec3 vBossLeft;

	D3DXVec3Cross(&vBossLeft, &vBossUp, &vBossLook);
	D3DXVec3Normalize(&vBossUp, &vBossUp);
	D3DXVec3Normalize(&vBossLeft, &vBossLeft);

	_vec3 vBossRight = -vBossLeft;
	_vec3 vBossDown = -vBossUp;

	_vec3 vStartPos = vBossPos + (-vBossLook * 1.5f) + (vBossRight * 4.f) + (vBossDown * 9.f);

	m_pTransformCom->Set_Pos(vStartPos);

	__super::Update_GameObject(fTimeDelta);
	if (INFO.bDead) {
		//CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
		//MBEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		//Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
		//MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		CDustGrey* DustParticle = CDustGrey::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], 64);
		DustParticle->Set_ObjectTag(OBJECTTAG::PARTICLE);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::PARTICLE, DustParticle);
		DustParticle->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		CStage1Boss_BrifBigShield::m_bcheck2 = true;
		return OBJ_DEAD;
	}   // 사망판정
	Add_RenderGroup(RENDER_ALPHATEST, this);
	return OBJ_NOEVENT;
}

void CBossBigDaddy2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
	


}

void CBossBigDaddy2::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (INFO.bHit)
		m_iTextureIndex = 1;
	else
		m_iTextureIndex = 0;

	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer(INFO.MonsterState->Get_CurFrame(), INFO.MonsterState->Get_Ver());

}

void CBossBigDaddy2::ReadyState()
{
	m_pStateArray[IDLE] = new CBossBigDaddy_Idle;
	m_pStateArray[ATTACK] = new CBossBigDaddy_Attack;
}

void CBossBigDaddy2::OnCollisionEnter(CCollider* _pOther)
{


	//__super::OnCollisionEnter(_pOther);



}

void CBossBigDaddy2::OnCollisionStay(CCollider* _pOther)
{
	//__super::OnCollisionStay(_pOther);


}

void CBossBigDaddy2::OnCollisionExit(CCollider* _pOther)
{
	//__super::OnCollisionStay(_pOther);
}

HRESULT CBossBigDaddy2::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossBigDaddy"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::ProtoMgr()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

CBossBigDaddy2* CBossBigDaddy2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CBossBigDaddy2* pInstance = new CBossBigDaddy2(pGraphicDev);
	pInstance->m_iIndex = iIndex;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BrifMonsterBig Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CBossBigDaddy2::Free()
{


	for(auto iter : m_pStateArray)
		Safe_Delete(iter);

	//Safe_Release(m_pUI_Recognition);

//    Safe_Delete(INFO.MonsterState);
	__super::Free();
}
