#include "stdafx.h"
#include "KickBoardMonster.h"

#include "KickBoardMonster_Idle.h"
#include "KickBoardMonster_Chase.h"
#include "KickBoardMonster_Attack.h"
#include "KickBoardMonster_Patrol.h"
#include "KickBoardMonster_Dead.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MonsterBombEffect.h"
#include "MonsterState.h"


CKickBoardMonster::CKickBoardMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    :CMonster(pGraphicDev)
{
}

CKickBoardMonster::CKickBoardMonster(CMonster& rhs)
    : CMonster(rhs)
{
}

CKickBoardMonster::~CKickBoardMonster()
{
}

HRESULT CKickBoardMonster::Ready_GameObject()
{
    __super::Ready_GameObject();

    Set_ObjectTag(OBJECTTAG::MONSTER);
    INFO.iMobType = MonsterType::KCIKBOARD;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    ReadyState();

    INFO.MonsterState = m_pStateArray[IDLE];
    INFO.MonsterState->Initialize(this);
    INFO.fHP = 100.f;
    INFO.fMaxHP = 100.f;
    INFO.vPos = { 200.f,4.f,30.f }; // 470이엇음

    m_fDectedRange = 70.f;
    m_fAttackRange = 20.f;
    m_fSpeed = 25.f;


    m_pTransformCom->Set_Scale({ 5.0f,5.0f, 5.0f });
    m_pTransformCom->Set_Pos(INFO.vPos);

    m_pBufferCom->SetCount(5, 4);
    m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/neonshirt-v1_Resize_Hit.png", 1);


	m_pCollider->Set_Host(this);
	m_pCollider->Set_Transform(m_pTransformCom);
	m_pRigidBody->Set_Host(this);
	m_pRigidBody->Set_Transform(m_pTransformCom);


	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());
    m_eHitType = BULLETTYPE::SHOTGUN_RAZER;
    m_pMonsterBullet = nullptr;
    return S_OK;
}

_int CKickBoardMonster::Update_GameObject(const _float& fTimeDelta)
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

void CKickBoardMonster::LateUpdate_GameObject()
{


    if (INFO.bDead) {
		CMonsterBombEffect* MBEffect = CMonsterBombEffect::Create(m_pGraphicDev);
		MBEffect->Set_ObjectTag(OBJECTTAG::EFFECT);
		Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT, MBEffect);
		MBEffect->Get_Transform()->Set_Pos(m_pTransformCom->m_vInfo[INFO_POS]);
        INFO.MonsterState = m_pStateArray[DEAD];
        INFO.MonsterState->Initialize(this);
        INFO.bDead = false;
    }   // 사망판정

    __super::LateUpdate_GameObject();

    //_vec3	vPos;
    //m_pTransformCom->Get_Info(INFO_POS, &vPos);
    //__super::Compute_ViewZ(&vPos);
}

void CKickBoardMonster::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    INFO.MonsterState->Render(this);
}

void CKickBoardMonster::ReadyState()
{
	m_pStateArray[IDLE] = new CKickBoardMonster_Idle;
	m_pStateArray[CHASE] = new CKickBoardMonster_Chase;
	m_pStateArray[ATTACK] = new CKickBoardMonster_Attack;
    m_pStateArray[PATROL] = new CKickBoardMonster_Patrol;
	m_pStateArray[DEAD] = new CKickBoardMonster_Dead;
}

void CKickBoardMonster::OnCollisionEnter(CCollider* _pOther)
{
    __super::OnCollisionEnter(_pOther);

    // 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

    if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
    {

        cout << "워리어 공격" << endl;
    }
}

void CKickBoardMonster::OnCollisionStay(CCollider* _pOther)
{

    __super::OnCollisionStay(_pOther);

	// 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .
}

void CKickBoardMonster::OnCollisionExit(CCollider* _pOther)
{
}

void CKickBoardMonster::Ride_On(_vec3 _vDir, _float _fSpeed, _float _fTimeDelta)
{
    _vec3 vDir = _vDir;
    D3DXVec3Normalize(&vDir, &vDir);
    _float fMobAngle = atan2f(vDir.x, vDir.z);
    Set_Pos(INFO.vPos + vDir * _fSpeed * _fTimeDelta);

    _vec3	vPlayerPos, vRel, vPlayerPos_Rel;
    m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

    vRel = vPlayerPos - Get_Info().vPos; //플레이어와 몬스터의 각도 구함
    vPlayerPos_Rel = vRel - vDir;
    D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);

    _float fPlayerAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);

    fPlayerAngle = D3DXToDegree(fPlayerAngle - fMobAngle);

    fPlayerAngle = ((_int)fPlayerAngle+360) % 360;

    if ((fPlayerAngle >= 0 && fPlayerAngle <= 45) || (fPlayerAngle > 315 && fPlayerAngle <= 360)) {
        m_fAdditional_frame = 0;
    }
    else if (fPlayerAngle > 45 && fPlayerAngle <= 135) {
        m_fAdditional_frame = 4;
    }
    else if (fPlayerAngle > 135 && fPlayerAngle <= 225) {
        m_fAdditional_frame = 6;
    }
    else if (fPlayerAngle > 225 && fPlayerAngle <= 315) {
        m_fAdditional_frame = 2;
    }
    else {

        int a = 1 + 1;
    }

    Set_Frame(4, 5, m_fCurFrame + m_fAdditional_frame);
    m_fAnimateTime += _fTimeDelta;
    if (m_fAnimateTime >= 0.05f)
    {
        ++m_fCurFrame;
        m_fAnimateTime = 0;
    }//프레임 코드



    if (m_fCurFrame >= m_fMaxFrame) {
        m_fCurFrame = m_fMinFrame;
    }

}

HRESULT CKickBoardMonster::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_KickBoardMonster"));
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

CKickBoardMonster* CKickBoardMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CKickBoardMonster* pInstance = new CKickBoardMonster(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("KickBoardMonster Create Failed");

        return nullptr;
    }
    return pInstance;
}

void CKickBoardMonster::Free()
{
	for (auto iter : m_pStateArray)
		Safe_Delete(iter);

    __super::Free();
}
