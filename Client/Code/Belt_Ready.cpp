#include "stdafx.h"
#include "Belt_Ready.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBelt_Ready::CBelt_Ready()
{


}

CBelt_Ready::~CBelt_Ready()
{
}
void CBelt_Ready::Initialize(CBelt* Belt)
{
    m_fBehaviorTime = 0.f;
    m_fMoveDownSum = - 0.1f;
    m_fRotationMax = D3DXToRadian(-30);
    m_fRotate = D3DXToRadian(1);
    m_bAttack = false;

    m_pHost = Belt;
    Belt->m_pTransformCom->Rotation(ROT_Z, m_fRotationMax);
}

CBeltState* CBelt_Ready::Update(CBelt* Belt, const float& fTimeDelta)
{

	//                  0       1       2           3       4       5
		//enum BELTSTATE { IDLE, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END };

		//todo Key_Pressing 눌리고 있는 경우
		//todo Key_Down 이전에 눌린적이 없고 현재 눌렀을 경우
		//todo Key_Up 이전에는 눌린 적이 있고 현재 눌리지 않았을 경우

		m_fBehaviorTime += fTimeDelta;

		if (m_fBehaviorTime >= 0.02f) {
			//Belt->m_fBeltMoveRight += m_fMoveRightSum;
			if (Belt->m_fBeltMoveDown > 1.5f) {
				Belt->m_fBeltMoveDown += m_fMoveDownSum;
				Belt->m_pTransformCom->Rotation(ROT_Z, m_fRotate);
			}
		}

	     if (Key_Pressing(DIK_LSHIFT))
	     {
	             if(m_fBehaviorTime > m_fChargeTime)
	 			return m_pHost->Get_State(4); // CHAGE
	     }
	     else
	     {
	         m_fBehaviorTime = 0.f;
	     }

	 	 if (Key_Up(DIK_LSHIFT))
	 	{
	 		m_fBehaviorTime = 0.f;
			if (RopeActionTest())
			{
				return m_pHost->Get_State(5); // Rope 테스트
			}
			else
	 		return m_pHost->Get_State(2); // attack;
	 	}

    return nullptr;
}

void CBelt_Ready::Release(CBelt* Belt)
{
   // Belt->m_fBeltMoveRight = 4;
   //Belt->m_fBeltMoveDown = 1.5f;
    //Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
   // Belt->m_vBeltScale = { 0.11f,0.11f,0.11f };
   m_fBehaviorTime = 0.f;
}

_bool CBelt_Ready::RopeActionTest()
{
	_vec3 vStartPos = m_pHost->Get_StartPos();
	auto& MonsterList = Management()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::MONSTER);

	if (!MonsterList.empty())
	{
		for (auto iter : MonsterList)
		{
			if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), iter->Get_Collider(), m_pHost->Get_StartPos()))
			{
				m_pHost->Set_Target(iter);
				return true;
			}
		}
	}

	return false;

}
