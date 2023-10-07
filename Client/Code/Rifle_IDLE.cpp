#include "stdafx.h"
#include "..\Header\Rifle_IDLE.h"
#include "Rifle_WALK.h"
#include "Rifle_RELOAD.h"
#include "Rifle_READY.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Rifle_IDLE_Walk.h"
//#include "..\Header\Rifle_IDLE_Attack.h"

CRifle_IDLE::CRifle_IDLE()
{


}

CRifle_IDLE::~CRifle_IDLE()
{
}
void CRifle_IDLE::Initialize(CGun* Rifle)
{
    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    NULL_CHECK(pPlayerTransCom);
    _vec3   vPlayerPos;
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

    m_vPrePos = vPlayerPos;

    m_bAttack = false;
}

CRifleState* CRifle_IDLE::Update(CGun* Rifle, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;
    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    NULL_CHECK(pPlayerTransCom);
    _vec3   vPlayerPos;
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

    if (m_vPrePos != vPlayerPos) {
        return new CRifle_WALK;
    }
    m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김

    if (Rifle->m_bReload == true) {
        return new CRifle_RELOAD;
    }

    dynamic_cast<CRifle*>(Rifle)->Gun_Fire();

    if (Rifle->m_bReady) {
        return new CRifle_READY;
    }

    return nullptr;
}

void CRifle_IDLE::Release(CGun* Rifle)
{
}
