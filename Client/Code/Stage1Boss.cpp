#include "stdafx.h"
#include "Stage1Boss.h"
#include "Bullet_GoldBar.h"
#include "Player.h"

#include "Stage1Boss_Idle.h"
#include "Stage1Boss_ThrowGoldSingle.h"
#include "Stage1Boss_Jump.h"
#include "Stage1Boss_MonsterSpawn.h"
#include "Stage1Boss_ThrowGoldMulti.h"
#include "Stage1Boss_BrifBigShield.h"
#include "Stage1Boss_Dead.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "MonsterBombEffect.h"
#include "BossState.h"


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
	__super::Ready_GameObject();

	Set_ObjectTag(OBJECTTAG::BOSS);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	ReadyState();


	INFO.MonsterState = m_pStateArray[IDLE];
	INFO.MonsterState->Initialize(this);
	INFO.fHP = 1000.f;
	INFO.fMaxHP = 1000.f;



	m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/hugo bauss transformed1.png", 1);

	m_pBufferCom->SetCount(5, 5);

	m_pCollider->Set_Host(this);
	m_pRigidBody->Set_Host(this);
	m_pTransformCom->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pRigidBody->Set_Transform(m_pTransformCom);

	m_pTransformCom->Set_Pos((_vec3{ 92.5f,50.f,125.f }));
	m_pTransformCom->Set_Scale({ 10.0f,13.0f,10.0f });
	m_pTransformCom->Translate(_vec3(0.f, 1.f, 0.f));

	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());
	return S_OK;
}

_int CStage1Boss::Update_GameObject(const _float& fTimeDelta)
{
	//m_pCollider->SetCenterPos(m_pTransformCom->m_vInfo[INFO_POS]);
	m_pRigidBody->Update_RigidBody(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CStage1Boss::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	// 사망판정
	if (INFO.bDead) {
		CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
		MBEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
		MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);
		INFO.MonsterState = m_pStateArray[DEAD];
		INFO.MonsterState->Initialize(this);
		INFO.bDead = false;
	}   // 사망판정

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);

}

void CStage1Boss::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (INFO.bHit)
		m_iTextureIndex = 1;
	else
		m_iTextureIndex = 0;

	m_pTextureCom->Render_Textrue(m_iTextureIndex);
	m_pBufferCom->Render_Buffer(INFO.MonsterState->Get_CurFrame(), INFO.MonsterState->Get_Ver());
}

void CStage1Boss::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

}

void CStage1Boss::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);

	// 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

}

void CStage1Boss::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
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


void CStage1Boss::Chase_Target(_float fTimeDelta, _float _fSpeed)
{

	_vec3 vPlayerPos, vMyPos, vPlayerPos_Rel;
	_float fSpeed;

	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	D3DXVec3Normalize(&_vec3(vPlayerPos - vMyPos), &vPlayerPos_Rel);

	fSpeed = _fSpeed;

	m_pTransformCom->Chase_Target(&vPlayerPos, fTimeDelta, fSpeed);
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
