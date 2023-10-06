#include "stdafx.h"
#include "..\Header\Rifle_WALK.h"

#include "Rifle_IDLE.h"
#include "Rifle_RELOAD.h"
#include "Rifle_READY.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Rifle_WALK_Walk.h"
//#include "..\Header\Rifle_WALK_Attack.h"

CRifle_WALK::CRifle_WALK()
{


}

CRifle_WALK::~CRifle_WALK()
{
}
void CRifle_WALK::Initialize(CGun* Rifle)
{
    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    NULL_CHECK(pPlayerTransCom);
    _vec3   vPlayerPos;
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

    m_vPrePos = vPlayerPos;

    m_bAttack = false;

    m_fMoveRightSum = -0.021f;
    m_fMoveDownSum = -0.01f;

}

CRifleState* CRifle_WALK::Update(CGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    if (m_fBehaviorTime >= 0.02f) {
        Rifle->m_fGunMoveRight += m_fMoveRightSum;
        Rifle->m_fGunMoveDown += m_fMoveDownSum;
        if (Rifle->m_fGunMoveRight >= 3.2f || Rifle->m_fGunMoveRight < 2.8f) {
            m_fMoveRightSum = -m_fMoveRightSum;
            m_fMoveDownSum = -m_fMoveDownSum;// 방향 반전
        }
        if (Rifle->m_fGunMoveRight < 3.009f && Rifle->m_fGunMoveDown > 2.991f) {
            Rifle->m_fGunMoveRight = 3.f;
            Rifle->m_fGunMoveDown = 1.f;
            m_fMoveDownSum = -m_fMoveDownSum;
        }

        m_fBehaviorTime = 0.f;
    }


    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    NULL_CHECK(pPlayerTransCom);
    _vec3   vPlayerPos;
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
    if (m_vPrePos == vPlayerPos) {
        return new CRifle_IDLE;
    }
    m_vPrePos = vPlayerPos;
    if (Rifle->m_bReload == true) {
        return new CRifle_RELOAD;
    }
    dynamic_cast<CRifle*>(Rifle)->Gun_Fire();

    if (Rifle->m_bReady) {
        return new CRifle_READY;
    }

    return nullptr;
}

void CRifle_WALK::Release(CGun* Rifle)
{
    Rifle->m_fGunMoveRight = 3.f;
    Rifle->m_fGunMoveDown = 1.f;
}
