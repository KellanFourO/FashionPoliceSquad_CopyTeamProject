#include "stdafx.h"
#include "Belt_Rope.h"
#include "BeltState.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBelt_Rope::CBelt_Rope()
{


}

CBelt_Rope::~CBelt_Rope()
{
}
void CBelt_Rope::Initialize(CBelt* Belt)
{
    m_bAttack = false;
    m_pHost = Belt;
}

CBeltState* CBelt_Rope::Update(CBelt* Belt, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

    _vec3   vPlayerPos;
    m_pHost->Get_HostTransform()->Get_Info(INFO_POS, &vPlayerPos);
    if (m_vPrePos != vPlayerPos) {
        return m_pHost->Get_State(0); // IDLE
    }
    m_vPrePos = vPlayerPos; // 이전 좌표랑 비교해서 숫자가 다르면 WALK로 넘김

    return nullptr;
}

void CBelt_Rope::Release(CBelt* Belt)
{
}
