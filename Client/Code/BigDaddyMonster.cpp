#include "stdafx.h"
#include "BigDaddyMonster.h"

#include "BigDaddyMonster_IDLE.h"
#include "BigDaddyMonster_Chase.h"
#include "BigDaddyMonster_Dead.h"
#include "BigDaddyMonster_Attack.h"
#include "DustGrey.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "MonsterBombEffect.h"
#include "MonsterState.h"

CBigDaddyMonster::CBigDaddyMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CBigDaddyMonster::CBigDaddyMonster(CMonster& rhs)
	: CMonster(rhs)
{
}

CBigDaddyMonster::~CBigDaddyMonster()
{
}

HRESULT CBigDaddyMonster::Ready_GameObject(_vec3 pPoint)
{
	__super::Ready_GameObject();

	m_StartingPoint = pPoint;
	Set_ObjectTag(OBJECTTAG::MONSTER);
	INFO.iMobType = MonsterType::BIGDADDY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	ReadyState();

	INFO.MonsterState = m_pStateArray[IDLE];
	INFO.MonsterState->Initialize(this);
	INFO.fHP = 100.f;
	INFO.fMaxHP = 100.f;

	//원래 150, 100이었는데 80, 60으로 수정해봄 - 유진
	m_fDectedRange = 80.f; //! 탐색 범위
	m_fAttackRange = 60.f; //! 공격 범위

	m_pTransformCom->Set_Scale({ 5.0f,5.0f,5.0f });

	//Set_Pos((_vec3{ 120.f,4.f,28.f }));
	//m_pTransformCom->Set_Pos((_vec3{ 198.5f,10.0f,235.5f }));
	m_pTransformCom->Set_Pos(m_StartingPoint);

	m_pBufferCom->SetCount(4, 4);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/loose-suit-spritesheet_hit.png", 1);

	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pRigidBody->Set_Host(this);
	m_pRigidBody->Set_Transform(m_pTransformCom);


	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());
	m_eHitType = BULLETTYPE::ASSERTRIFLE_BULLET;
	m_pMonsterBullet = nullptr;


	return S_OK;
}

_int CBigDaddyMonster::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);

	if (INFO.bRealDead && m_eObjectTag != OBJECTTAG::BOSS)
	{
		m_fDeadTick += fTimeDelta;

		if (m_fDeadTick > 3)
		{
			return OBJ_DEAD;
		}
	}

	//m_pUI_Recognition->Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CBigDaddyMonster::LateUpdate_GameObject()
{

	if (INFO.bDead) {
		CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
		MBEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
		MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		CDustGrey* DustParticle = CDustGrey::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS],256);
		DustParticle->Set_ObjectTag(OBJECTTAG::PARTICLE);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::PARTICLE, DustParticle);
		DustParticle->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);


		INFO.MonsterState = m_pStateArray[DEAD];
		INFO.MonsterState->Initialize(this);
		INFO.bDead = false;
		INFO.bRealDead = true;
	}   // 사망판정

	__super::LateUpdate_GameObject();

}

void CBigDaddyMonster::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (INFO.bHit)
		m_iTextureIndex = 1;
	else
		m_iTextureIndex = 0;

	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer(INFO.MonsterState->Get_CurFrame(), INFO.MonsterState->Get_Ver());

}

void CBigDaddyMonster::ReadyState()
{
	m_pStateArray[IDLE] = new CBigDaddyMonster_Idle;
	m_pStateArray[CHASE] = new CBigDaddyMonster_Chase;
	m_pStateArray[DEAD] = new CBigDaddyMonster_Dead;
	m_pStateArray[ATTACK] = new CBigDaddyMonster_Attack;
}

void CBigDaddyMonster::OnCollisionEnter(CCollider* _pOther)
{


	__super::OnCollisionEnter(_pOther);


// 	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET)
// 	{
// 		cout << "워리어 공격" << endl;
// 	}
}

void CBigDaddyMonster::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);

	// 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

}

void CBigDaddyMonster::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

HRESULT CBigDaddyMonster::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_suit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::CALCULATOR, pComponent);

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

CBigDaddyMonster* CBigDaddyMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pPoint)
{
	CBigDaddyMonster* pInstance = new CBigDaddyMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPoint)))
	{
		Safe_Release(pInstance);
		MSG_BOX("BrifMonsterBig Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CBigDaddyMonster::Free()
{


	for(auto iter : m_pStateArray)
		Safe_Delete(iter);

	//Safe_Release(m_pUI_Recognition);

//    Safe_Delete(INFO.MonsterState);
	__super::Free();
}
