#include "stdafx.h"
#include "Stage1Boss.h"
#include "Bullet_GoldBar.h"
#include "Player.h"
#include "Build_Obj.h"
#include "BossStage.h"

#include "Stage1Boss_Idle.h"
#include "Stage1Boss_ThrowGoldSingle.h"
#include "Stage1Boss_Jump.h"
#include "Stage1Boss_MonsterSpawn.h"
#include "Stage1Boss_ThrowGoldMulti.h"
#include "Stage1Boss_BrifBigShield.h"
#include "Stage1Boss_Dead.h"

#include "Stage1BossAuraEffect.h"
#include "DustGrey.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "MonsterBombEffect.h"
#include "BossState.h"

_bool CStage1Boss::m_bBossPhase2 = false;
CStage1Boss::CStage1Boss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CStage1Boss::CStage1Boss(CMonster& rhs)
	: CMonster(rhs)
{
}

CStage1Boss::~CStage1Boss()
{
}

HRESULT CStage1Boss::Ready_GameObject()
{

	Set_ObjectTag(OBJECTTAG::BOSS);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	ReadyState();


	INFO.MonsterState = m_pStateArray[IDLE];
	INFO.MonsterState->Initialize(this);
	INFO.fHP = 4800.f;
	INFO.fMaxHP = 5000.f;
	m_fSpeed = 10.f;
	m_fDectedRange = 150.f;
	m_fAttackRange = 70.f;

	m_pTransformCom->Set_Pos((_vec3{ 92.5f,25.f,135.f }));
	m_pTransformCom->Set_Scale({ 15.0f,15.0f,15.0f });
	m_pBufferCom->SetCount(5, 5);

	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/boss 1 - hugo bauss sprite1_Hit.png", 1);
	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/hugo bauss transformed1.png", 1);


	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pRigidBody->Set_Host(this);
	m_pRigidBody->Set_Transform(m_pTransformCom);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());


	m_pMonsterBullet = nullptr;
	return S_OK;
}

_int CStage1Boss::Update_GameObject(const _float& fTimeDelta)
{


	__super::Update_GameObject(fTimeDelta);
	PhaseChange();
	return OBJ_NOEVENT;
}

void CStage1Boss::LateUpdate_GameObject()
{

	// 사망판정
	if (INFO.bDead) {
		CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
		MBEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
		MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		CDustGrey* DustParticle = CDustGrey::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], 256);
		DustParticle->Set_ObjectTag(OBJECTTAG::PARTICLE);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::PARTICLE, DustParticle);
		DustParticle->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);


		INFO.MonsterState->Release(this);
		INFO.MonsterState = m_pStateArray[DEAD];
		INFO.MonsterState->Initialize(this);
		INFO.bDead = false;
		INFO.bRealDead = true;
		m_bDead2 = true;
	}   // 사망판정
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CStage1Boss::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

//	m_pCollider->Render_Collider();
	if (INFO.bHit)
		m_iTextureIndex = 1;
	else if (m_bDead2)
		m_iTextureIndex = 2;
	else
		m_iTextureIndex = 0;


	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer(INFO.MonsterState->Get_CurFrame(), INFO.MonsterState->Get_Ver());
}

void CStage1Boss::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

// 	if(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BUILD_OBJ
// 		&& dynamic_cast<CBuild_Obj*>(_pOther)->Get_OBJ_ATTRIBUTE() == OBJ_ATTRIBUTE::INTER_OBJ)
// 	{
//
// 	}

}

void CStage1Boss::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);



	// 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

}

void CStage1Boss::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CStage1Boss::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Stage1Boss"));
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

void CStage1Boss::PhaseChange()
{
	_float fRatio = INFO.fHP / INFO.fMaxHP;



	if (fRatio < 0.6 && fRatio > 0.3)
	{
		dynamic_cast<CStage1BossState*>(INFO.MonsterState)->m_ePhase = BOSSPHASE::PHASE_2;
		if (!m_bMonsterSpawn)
		{
			INFO.MonsterState->Release(this);
			INFO.MonsterState = m_pStateArray[BRIFSHIELD];
			INFO.MonsterState->Initialize(this);
			m_bMonsterSpawn = true;

			dynamic_cast<CBossStage*>(Engine::Management()->Get_One_Scene(SCENETAG::BOSS_STAGE))->Create_Monster();
		}

	}
	else if (fRatio < 0.3)
	{
		dynamic_cast<CStage1BossState*>(INFO.MonsterState)->m_ePhase = BOSSPHASE::PHASE_3;
		if (!m_bOnAura)
		{
			CGameObject* pGameObject = CStage1BossAuraEffect::Create(m_pGraphicDev, this);
			Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, pGameObject);
			m_bOnAura = true;
		}
	}

}



void CStage1Boss::ReadyState()
{
	m_pStateArray[IDLE] = new CStage1Boss_Idle;
	m_pStateArray[JUMP] = new CStage1Boss_Jump;
	m_pStateArray[THROWSINGLE] = new CStage1Boss_ThrowGoldSingle;
	m_pStateArray[THROWMULTI] = new CStage1Boss_ThrowGoldMulti;
	m_pStateArray[BRIFSHIELD] = new CStage1Boss_BrifBigShield;
	m_pStateArray[SPAWN] = new CStage1Boss_MonsterSpawn;
	m_pStateArray[DEAD] = new CStage1Boss_Dead;
}

CStage1Boss* CStage1Boss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage1Boss* pInstance = new CStage1Boss(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Stage1Boss Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CStage1Boss::Free()
{
	for (auto iter : m_pStateArray)
	{
		Safe_Delete(iter);
	}


	__super::Free();
}
