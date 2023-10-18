#include "stdafx.h"
#include "DullSuitMonster.h"

#include "DullSuitMonster_IDLE.h"
#include "DullSuitMonster_Chase.h"
#include "DullSuitMonster_Dead.h"
#include "DullSuitMonster_Attack.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MonsterBombEffect.h"
#include "DustGrey.h"
#include "MonsterState.h"


CDullSuitMonster::CDullSuitMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    :CMonster(pGraphicDev)
{
}

CDullSuitMonster::CDullSuitMonster(CMonster& rhs)
    : CMonster(rhs)
{
}

CDullSuitMonster::~CDullSuitMonster()
{
}

HRESULT CDullSuitMonster::Ready_GameObject(_vec3 pPoint)
{
    __super::Ready_GameObject();

	m_StartingPoint = pPoint;

    INFO.iMobType = MonsterType::DULLSUIT;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    ReadyState();

    INFO.MonsterState = m_pStateArray[IDLE];
    INFO.MonsterState->Initialize(this);
    INFO.fHP = 80.f;
    INFO.fMaxHP = 80.f;
    //INFO.vPos = { 40.f,4.f,20.f };

    //유진 70, 50으로 수정함
    m_fDectedRange = 70.f;
    m_fAttackRange = 50.f;

    //m_pTransformCom->Set_Pos(INFO.vPos);
    m_pTransformCom->Set_Pos(m_StartingPoint);

    m_pTransformCom->Set_Scale(_vec3{3.f,4.5f,3.f });

    m_pBufferCom->SetCount(4,4);
    m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/dull_suit4_hit.png", 1);

    m_pCollider->Set_Host(this);
    m_pCollider->Set_Transform(m_pTransformCom);
    m_pRigidBody->Set_Host(this);
    m_pRigidBody->Set_Transform(m_pTransformCom);
    m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

    m_eHitType = BULLETTYPE::SHOTGUN_BULLET;
    m_pMonsterBullet = nullptr;


    return S_OK;
}

_int CDullSuitMonster::Update_GameObject(const _float& fTimeDelta)
{
    __super::Update_GameObject(fTimeDelta);
// 	if (INFO.bDead)
// 	{
// 		CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
// 		Management()->Get_Layer(LAYERTAG::EFFECT)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
// 		MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);
// 	}
	m_pRigidBody->Update_RigidBody(fTimeDelta);
    return OBJ_NOEVENT;
}

void CDullSuitMonster::LateUpdate_GameObject()
{


     if (INFO.bDead)
     {
		 CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
		 MBEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		 Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
		 MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

		 CDustGrey* DustParticle = CDustGrey::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], 256);
		 DustParticle->Set_ObjectTag(OBJECTTAG::PARTICLE);
		 Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::PARTICLE, DustParticle);
		 DustParticle->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);

         INFO.MonsterState = m_pStateArray[DEAD];
         INFO.MonsterState->Initialize(this);
         INFO.bDead = false;
     }   // 사망판정
    __super::LateUpdate_GameObject();



	//_vec3	vPos;
	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);
}

void CDullSuitMonster::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    if (INFO.bHit)
        m_iTextureIndex = 1;
    else
        m_iTextureIndex = 0;

    m_pTextureCom->Render_Textrue(m_iTextureIndex);
    m_pBufferCom->Render_Buffer(INFO.MonsterState->Get_CurFrame(),INFO.MonsterState->Get_Ver());

}

void CDullSuitMonster::ReadyState()
{
    m_pStateArray[IDLE] = new CDullSuitMonster_Idle;
    m_pStateArray[CHASE] = new CDullSuitMonster_Chase;
    m_pStateArray[DEAD] = new CDullSuitMonster_Dead;
    m_pStateArray[ATTACK] = new CDullSuitMonster_Attack;
}

void CDullSuitMonster::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

    // 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

    if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
    {

        cout << "워리어 공격" << endl;
    }


}

void CDullSuitMonster::OnCollisionStay(CCollider* _pOther)
{
    __super::OnCollisionStay(_pOther);
}

void CDullSuitMonster::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CDullSuitMonster::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_suit_2"));
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

CDullSuitMonster* CDullSuitMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pPoint)
{
    CDullSuitMonster* pInstance = new CDullSuitMonster(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject(pPoint)))
    {
        Safe_Release(pInstance);
        MSG_BOX("DullSuitMonster Create Failed");

        return nullptr;
    }
    return pInstance;
}

void CDullSuitMonster::Free()
{
	for (auto iter : m_pStateArray)
		Safe_Delete(iter);

    __super::Free();
}
