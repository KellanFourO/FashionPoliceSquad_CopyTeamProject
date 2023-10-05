#include "stdafx.h"
#include "..\Header\Belt_NORMAL.h"
#include "BeltState.h"
#include "Belt_NORMAL.h"
#include "Belt_ATTACK.h"
#include "Belt_CHARGE.h"

#include "Export_System.h"
#include "Export_Utility.h"

//#include "BrifCase.h"
//#include "..\Header\Belt_NORMAL_Walk.h"
//#include "..\Header\Belt_NORMAL_Attack.h"

CBelt_NORMAL::CBelt_NORMAL()
{


}

CBelt_NORMAL::~CBelt_NORMAL()
{
}
void CBelt_NORMAL::Initialize(CBelt* Belt)
{
    m_fBehaviorTime = 0.f;
    m_fMoveDownSum = - 0.1f;
    m_fRotationMax = D3DXToRadian(-30);
    m_fRotate = D3DXToRadian(1);    
    m_bAttack = false;

    Belt->m_pTransformCom->Rotation(ROT_Z, m_fRotationMax);
}

CBeltState* CBelt_NORMAL::Update(CBelt* Belt, const float& fDeltaTime)
{
    m_fBehaviorTime += fDeltaTime;
    if (m_fBehaviorTime >= 0.02f) {
        //Belt->m_fBeltMoveRight += m_fMoveRightSum;
        if (Belt->m_fBeltMoveDown > 1.5f) {
            Belt->m_fBeltMoveDown += m_fMoveDownSum;
            Belt->m_pTransformCom->Rotation(ROT_Z, m_fRotate);
        }
    }
    if (!(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)) {
        return new CBelt_ATTACK;
    }
    if (m_fBehaviorTime >= 2.f) {
        if (Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80) {
            return new CBelt_CHARGE;
        }
    }

   
    //return new CBelt_NORMAL;
   

    return nullptr;
}

void CBelt_NORMAL::Release(CBelt* Belt)
{
   // Belt->m_fBeltMoveRight = 4;
   //Belt->m_fBeltMoveDown = 1.5f;
    //Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
   // Belt->m_vBeltScale = { 0.11f,0.11f,0.11f };
}
