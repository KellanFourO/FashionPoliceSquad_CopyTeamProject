#include "stdafx.h"
#include "Belt_Attack.h"
#include "BeltState.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"

CBelt_Attack::CBelt_Attack()
{


}

CBelt_Attack::~CBelt_Attack()
{
}
void CBelt_Attack::Initialize(CBelt* Belt)
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

    m_bAttack = true;

    m_pHost = Belt;

    Belt->m_fBeltMoveRight -= m_fMoveRightMax;
    Belt->m_fBeltMoveDown -= m_fMoveUpMax;

    Belt->m_pTransformCom->RotateAxis(Belt->m_vPlayerLook, D3DXToRadian(m_fRotateMax));

    SoundMgr()->PlaySoundW(L"Player_BeltAttack.wav",SOUND_PLAYER2, 1.f);
}

CBeltState* CBelt_Attack::Update(CBelt* Belt, const float& fTimeDelta)
{
    m_fBehaviorTime += fTimeDelta;

	//                  0       1       2           3       4       5
	//enum BELTSTATE { IDLE, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END };

    if (TargetStateChange())
    {
        return m_pHost->Get_State(5);
    }

    auto& MonsterList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::MONSTER);
    auto& BossList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC,OBJECTTAG::BOSS);

    if (!MonsterList.empty())
    {
        for (auto iter : MonsterList)
        {
            if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), iter->Get_Collider(), m_pHost->Get_StartPos()))
            {
                if (m_bAttack)
                {
                    static_cast<CMonster*>(iter)->Attacked(m_pHost->m_fDmg);
                    m_bAttack = false;
                }
            }
        }
    }

    if (!BossList.empty())
    {
        if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), BossList.back()->Get_Collider(), m_pHost->Get_StartPos()))
        {
            if (m_bAttack)
            {
                static_cast<CMonster*>(BossList.back())->Attacked(m_pHost->m_fDmg);
                m_bAttack = false;
            }
        }
    }


    if (m_fBehaviorTime >= 0.3f)
    {
        _vec3 vStartPos, vEndPos, vPlayerUp;
        vStartPos = m_pHost->Get_StartPos();
        vEndPos = m_pHost->Get_EndPos();

        m_pHost->Get_HostTransform()->Get_Info(INFO_UP, &vPlayerUp);

        _float fLength = 0.f;

        fLength = D3DXVec3Length(&(vEndPos - vStartPos));
        Belt->m_vBeltScale *= 1.007f;

        Belt->m_vBeltScale.z = Belt->m_vBeltScale.z + fLength;


        Belt->m_fBeltMoveRight -= m_fMoveRightSum;
        Belt->m_fBeltMoveDown += m_fMoveDownSum;
        Belt->m_bHit = false;
        Belt->m_pTransformCom->RotateAxis(vPlayerUp, D3DXToRadian(m_fRotateMax += m_fRotate * fTimeDelta));



        if (m_fBehaviorTime >= 0.8){
            return m_pHost->Get_State(0); // IDLE
        }
    }

    return nullptr;
}

void CBelt_Attack::Release(CBelt* Belt)
{
    Belt->m_bHit = false;
    Belt->m_bCharged = false;
    Belt->m_fBeltMoveRight = 4.f;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_vBeltScale = { 0.3f,0.3f,0.3f };

    Belt->m_pTransformCom->RotateAxis(_vec3{ 0.f,0.f,0.f }, 0);

    m_fBehaviorTime = 0.f;
    m_bAttack = true;
    //Belt->m_vBeltScale = { 0.11f,0.11f,0.11f };
    SoundMgr()->StopSound(SOUND_PLAYER2);
}

_bool CBelt_Attack::TargetStateChange()
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
