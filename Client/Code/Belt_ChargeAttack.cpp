#include "stdafx.h"
#include "Belt_ChargeAttack.h"
#include "BeltState.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"

CBelt_ChargeAttack::CBelt_ChargeAttack()
{


}

CBelt_ChargeAttack::~CBelt_ChargeAttack()
{
}
void CBelt_ChargeAttack::Initialize(CBelt* Belt)
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

    m_pHost = Belt;

    Belt->m_fBeltMoveRight -= m_fMoveRightMax;
    Belt->m_fBeltMoveDown -= m_fMoveUpMax;

    Belt->m_pTransformCom->RotateAxis(Belt->m_vPlayerLook, D3DXToRadian(m_fRotateMax));

    SoundMgr()->PlaySoundW(L"Player_BeltChargeAttack.wav",SOUND_PLAYER2,1);
}

CBeltState* CBelt_ChargeAttack::Update(CBelt* Belt, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

	//                  0       1       2           3       4       5
	//enum BELTSTATE { IDLE, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END };

    if (TargetStateChange())
    {
        return m_pHost->Get_State(4);
    }

    auto& MonsterList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::MONSTER);
    auto& BossList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::BOSS);

    if (!MonsterList.empty())
    {
        for (auto iter : MonsterList)
        {
            if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), iter->Get_Collider(), m_pHost->Get_StartPos()))
            {
                static_cast<CMonster*>(iter)->Attacked(m_pHost->m_fDmg);
                return m_pHost->Get_State(0); // IDLE
            }
        }
    }

    if (!BossList.empty())
    {
        if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), BossList.back()->Get_Collider(), m_pHost->Get_StartPos()))
        {
            static_cast<CMonster*>(BossList.back())->Attacked(m_pHost->m_fDmg);
        }
    }


    if (m_fBehaviorTime >= 0.3f)
    {
        Belt->m_vBeltScale *= 1.007f;
        Belt->m_fBeltMoveRight -= m_fMoveRightSum;
        Belt->m_fBeltMoveDown += m_fMoveDownSum;
        Belt->m_bHit = false;
        Belt->m_pTransformCom->RotateAxis(Belt->m_vPlayerLook, D3DXToRadian(m_fRotateMax += m_fRotate * fTimeDelta));



        if (m_fBehaviorTime >= 0.8){
            return m_pHost->Get_State(0); // IDLE
        }
    }

    return nullptr;
}

void CBelt_ChargeAttack::Release(CBelt* Belt)
{
    Belt->m_bHit = false;
    Belt->m_bCharged = false;
    Belt->m_fBeltMoveRight = 4.f;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_vBeltScale = { 0.3f,0.3f,0.3f };

    Belt->m_pTransformCom->RotateAxis(_vec3{ 0.f,0.f,0.f }, 0);

    SoundMgr()->StopSound(SOUND_PLAYER2);

    //Belt->m_vBeltScale = { 0.11f,0.11f,0.11f };
}

_bool CBelt_ChargeAttack::TargetStateChange()
{
    _vec3 vStartPos = m_pHost->Get_StartPos();

    auto& RopeList = Management()->Get_ObjectList(LAYERTAG::ENVIRONMENT,OBJECTTAG::BUILD_OBJ);

    for (auto iter : RopeList)
    {
        if (dynamic_cast<CBuild_Obj*>(iter)->Get_OBJ_Interaction() == OBJ_INTERACTION::OBJ_LOCK_OFF)
        {
            if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), iter->Get_Collider(), vStartPos))
            {
                m_pHost->Set_TargetObj(static_cast<CBuild_Obj*>(iter));
                return true;
            }
            else
            {
                m_pHost->Set_TargetObj(nullptr);
                continue;
            }
        }
    }

    if(!m_pHost->Get_TargetObj())
        return false;
}
