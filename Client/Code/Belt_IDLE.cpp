#include "stdafx.h"
#include "Belt_Idle.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBelt_Idle::CBelt_Idle()
{


}

CBelt_Idle::~CBelt_Idle()
{
}

void CBelt_Idle::Initialize(CBelt* Belt)
{
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);
    m_bAttack = false;
    m_pHost = Belt;
}

CBeltState* CBelt_Idle::Update(CBelt* Belt, const float& fTimeDelta)
{

    //                  0       1       2           3       4       5
    //enum BELTSTATE { IDLE, ATTACK, CHARGEATTACK, CHARGE, ROPE, BELTSTATE_END };

    //todo Key_Pressing 눌리고 있는 경우
    //todo Key_Down 이전에 눌린적이 없고 현재 눌렀을 경우
    //todo Key_Up 이전에는 눌린 적이 있고 현재 눌리지 않았을 경우

	_vec3 vStartPos = m_pHost->Get_StartPos();
	auto& ObjList = Management()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::BUILD_OBJ);



	if (!ObjList.empty())
	{
		for (auto iter : ObjList)
		{
			if (dynamic_cast<CBuild_Obj*>(iter)->Get_OBJ_Interaction() == OBJ_INTERACTION::OBJ_LOCK_OFF)
			{
			    if (CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), iter->Get_Collider(), m_pHost->Get_StartPos()))
			    {
			    	m_pHost->Set_Target(iter);
			    }
            }
		}
	}

    if (m_pHost->Get_Target() && !CollisionManager()->CollisionRayToCube(m_pHost->Get_Collider(), m_pHost->Get_Target()->Get_Collider(), m_pHost->Get_StartPos()))
    {
        m_pHost->Set_Target(nullptr);
    }


    if (Key_Down(DIK_LSHIFT))
    {
        return m_pHost->Get_State(1); // Ready
    }

    return nullptr;

}

void CBelt_Idle::Release(CBelt* Belt)
{
    Belt->m_fBeltMoveRight = 4;
    Belt->m_fBeltMoveDown = 5.f;
    Belt->m_pTransformCom->Set_Rotate(ROT_Z, 0);

}
