#include "stdafx.h"
#include "..\Header\Belt_ATTACK.h"
#include "BeltState.h"
#include "Belt_IDLE.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Belt_ATTACK_Walk.h"
//#include "..\Header\Belt_ATTACK_Attack.h"

CBelt_ATTACK::CBelt_ATTACK()
{


}

CBelt_ATTACK::~CBelt_ATTACK()
{
}
void CBelt_ATTACK::Initialize(CBelt* Belt)
{
    Belt->m_bHit = true;
    Belt->m_vBeltScale = { 0.2f,0.2f,0.2f };

    m_fMoveDownSum = -0.1f;

    Belt->m_fBeltMoveRight = 4.f;
    Belt->m_fBeltMoveDown = 1.2f;

    m_fMoveRightSum = -0.06f;
    m_fMoveDownSum = 0.013f;
    m_fMoveRightMax = 1.5f;
    m_fMoveUpMax = 0.16f;

    m_fRotateMax = -10.f;
    m_fRotate = 20.f;

    m_bAttack = false;

    Belt->m_fBeltMoveRight -= m_fMoveRightMax;
    Belt->m_fBeltMoveDown -= m_fMoveUpMax;
    
    Belt->m_pTransformCom->RotateAxis(Belt->m_vPlayerLook, D3DXToRadian(m_fRotateMax));
}

CBeltState* CBelt_ATTACK::Update(CBelt* Belt, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;
   // Belt->m_vBeltScale = { 0.21f,0.11f,0.11f };
    if (m_fBehaviorTime >= 0.3f) {
       
        //Belt->m_vBeltScale = { 0.11f,0.11f,0.11f };
       
        Belt->m_vBeltScale *= 1.007f;
        Belt->m_fBeltMoveRight -= m_fMoveRightSum;
        Belt->m_fBeltMoveDown += m_fMoveDownSum;
        Belt->m_bHit = false;
        Belt->m_pTransformCom->RotateAxis(Belt->m_vPlayerLook, D3DXToRadian(m_fRotateMax += m_fRotate * fDeltaTime));
        if (m_fBehaviorTime >= 0.8){
            return new CBelt_IDLE;
        }
    }
    else {
       
    }

    return nullptr;
}

void CBelt_ATTACK::Release(CBelt* Belt)
{
    Belt->m_bHit = false;
    Belt->m_bCharged = false;
    Belt->m_fBeltMoveRight = 4.f;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_vBeltScale = { 0.3f,0.3f,0.3f };

    Belt->m_pTransformCom->RotateAxis(_vec3{ 0.f,0.f,0.f }, 0);

    //Belt->m_vBeltScale = { 0.11f,0.11f,0.11f };
}
