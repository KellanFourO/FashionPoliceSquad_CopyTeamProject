#include "stdafx.h"
#include "BigDaddyMonster.h"

#include "BigDaddyMonster_IDLE.h"
#include "BigDaddyMonster_Chase.h"
#include "BigDaddyMonster_Dead.h"
#include "BigDaddyMonster_Attack.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "MonsterState.h"

// TODO - 승용 추가 : 몬스터 HP UI.
#include "UIMgr.h"

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

HRESULT CBigDaddyMonster::Ready_GameObject()
{
	//Set_ObjectTag(OBJECTTAG::MONSTER);
	__super::Ready_GameObject();

	INFO.iMobType = MonsterType::BRIFBIG;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	ReadyState();

	INFO.MonsterState = m_pStateArray[IDLE];
	INFO.MonsterState->Initialize(this);
	INFO.fHP = 100.f;
	INFO.fMaxHP = 100.f;

	m_fDectedRange = 150.f; //! 탐색 범위
	m_fAttackRange = 100.f; //! 공격 범위

	m_pTransformCom->Set_Scale({ 5.0f,5.0f,5.0f });
	Set_Pos((_vec3{ 25.f,4.f,28.f }));

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

	//m_pUI_Recognition->Update_GameObject(fTimeDelta);
	m_pRigidBody->Update_RigidBody(fTimeDelta);
	return OBJ_NOEVENT;
}

void CBigDaddyMonster::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (INFO.bDead) {
		INFO.MonsterState = m_pStateArray[DEAD];
		INFO.MonsterState->Initialize(this);
		INFO.bDead = false;
	}   // 사망판정

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);

	//m_pUI_Recognition->LateUpdate_GameObject();



}

void CBigDaddyMonster::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	INFO.MonsterState->Render(this);

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

CBigDaddyMonster* CBigDaddyMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBigDaddyMonster* pInstance = new CBigDaddyMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
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
