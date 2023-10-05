#include "stdafx.h"
#include "KickBoard.h"
#include "BrifCase.h"
#include "Player.h"

#include "KickBoard_IDLE.h"
#include "KickBoard_DEAD.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "MonsterState.h"


// TODO - 승용 추가 : 몬스터 HP UI.
#include "UIMgr.h"

CKickBoard::CKickBoard(LPDIRECT3DDEVICE9 pGraphicDev)
    :CMonster(pGraphicDev)
{
}

CKickBoard::CKickBoard(CMonster& rhs)
    : CMonster(rhs)
{
}

CKickBoard::~CKickBoard()
{
}

HRESULT CKickBoard::Ready_GameObject()
{
    Set_ObjectTag(OBJECTTAG::MONSTER);
    INFO.iMobType = MonsterType::KCIKBOARD;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Scale({ 5.0f,5.0f, 5.0f });


    m_pCollider->Set_Host(this);
    m_pCollider->Set_Transform(m_pTransformCom);
    m_pRigidBody->Set_Host(this);
    m_pRigidBody->Set_Transform(m_pTransformCom);
	m_pCollider->InitOBB(m_pTransformCom->m_vInfo[INFO_POS], &m_pTransformCom->m_vInfo[INFO_RIGHT], *m_pTransformCom->Get_Scale());

    INFO.bHit = false;

    INFO.MonsterState = new CKickBoard_IDLE();
    INFO.MonsterState->Initialize(this);

    INFO.vPos = { 470.f,4.f,30.f };
    Set_TransPos();
    m_pTransformCom->Translate(_vec3(0.f, 3.f, 0.f));

    // TODO - 승용 추가
    if (Set_HP() == E_FAIL)
    {
        MSG_BOX("승용 몬스터 HP 에러");
    }
    // TODO - 승용 추가 종료

   
    MonsterBullet = nullptr;

    m_pTextureCom->Ready_Texture(TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/neonshirt-v1_Hit.png", 1);

    INFO.fHP = 100.f;

    return S_OK;
}

_int CKickBoard::Update_GameObject(const _float& fTimeDelta)
{
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
    __super::Update_GameObject(fTimeDelta);


    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    NULL_CHECK_RETURN(pPlayerTransCom, -1);
    _vec3 vPlayerPos, vPlayerPos_Rel;
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
    vPlayerPos.y = 0.f;
    m_pTransformCom->Get_Info(INFO_POS, &INFO.vPos);
    vPlayerPos_Rel = vPlayerPos - INFO.vPos;
    D3DXVec3Normalize(&vPlayerPos_Rel, &vPlayerPos_Rel);
    _float fAngle = atan2f(vPlayerPos_Rel.x, vPlayerPos_Rel.z);
    m_pTransformCom->Set_Rotate(ROT_Y, fAngle + D3DX_PI);
    //방향전환 코드 (플레이어 방향)

    CMonsterState* State = INFO.MonsterState->Update(this, fTimeDelta);
    if (State != nullptr) {
        INFO.MonsterState->Release(this);
        Safe_Delete(INFO.MonsterState);
        INFO.MonsterState = State;
        INFO.MonsterState->Initialize(this);
    } // 상태 패턴

    m_pRigidBody->Update_RigidBody(fTimeDelta);
   
    return OBJ_NOEVENT;
}

void CKickBoard::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();


    if (INFO.bDead) {
        INFO.MonsterState->Release(this);
        Safe_Delete(INFO.MonsterState);
        INFO.MonsterState = new CKickBoard_DEAD;
        INFO.MonsterState->Initialize(this);
        INFO.bDead = false;
    }   // 사망판정
    
    
    _vec3	vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    __super::Compute_ViewZ(&vPos);
}

void CKickBoard::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pCollider->Render_Collider();

    INFO.MonsterState->Render(this);
}

void CKickBoard::OnCollisionEnter(CCollider* _pOther)
{
    if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER &&
        _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM &&
        _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYERBULLET)
        __super::OnCollisionEnter(_pOther);

    // 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

    if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
    {
        cout << "워리어 공격" << endl;
    }
}

void CKickBoard::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYERBULLET)
		__super::OnCollisionStay(_pOther);

	// 충돌 밀어내기 후 이벤트 여기다가 구현 ㄱㄱ ! .

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		cout << "워리어 공격" << endl;
	}
}

void CKickBoard::OnCollisionExit(CCollider* _pOther)
{
}

void CKickBoard::Ride_On(_vec3 _vDir, _float _fSpeed, _float _fTimeDelta)
{
    _vec3 vDir = _vDir;
    D3DXVec3Normalize(&vDir, &vDir);
    _float fMobAngle = atan2f(vDir.x, vDir.z);
    Set_Pos(INFO.vPos + vDir * _fSpeed * _fTimeDelta);
    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    _vec3	vPlayerPos, vRel, vPlayerPos_Rel;   
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
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

    Set_Frame(4, 4, m_fCurFrame + m_fAdditional_frame);
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

HRESULT CKickBoard::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CSYTex*>(Engine::Clone_Proto(L"Proto_SYTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_KickBoard"));
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

CKickBoard* CKickBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CKickBoard* pInstance = new CKickBoard(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("KickBoard Create Failed");

        return nullptr;
    }
    return pInstance;
}

void CKickBoard::Free()
{
    Safe_Release(m_pUI_HPFrame);
    Safe_Release(m_pUI_HPValue);

    Safe_Delete(INFO.MonsterState);
    __super::Free();
}
