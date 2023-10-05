#include "stdafx.h"
#include "..\Header\Belt_SWING.h"
#include "BeltState.h"
#include "Belt_SWING.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Belt_SWING_Walk.h"
//#include "..\Header\Belt_SWING_Attack.h"

CBelt_SWING::CBelt_SWING()
{


}

CBelt_SWING::~CBelt_SWING()
{
}
void CBelt_SWING::Initialize(CBelt* Belt)
{
    m_bAttack = false;
}

CBeltState* CBelt_SWING::Update(CBelt* Belt, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;
    CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
    NULL_CHECK(pPlayerTransCom);
    _vec3   vPlayerPos;
    pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
    if (m_vPrePos != vPlayerPos) {
        return new CBelt_SWING;
    }
    m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김

    return nullptr;
}

void CBelt_SWING::Release(CBelt* Belt)
{
}
