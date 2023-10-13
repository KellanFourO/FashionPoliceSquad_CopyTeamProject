#include "stdafx.h"
#include "KickBoardMonster_Attack.h"
#include "SYTex.h"
#include "Texture.h"
#include "Export_System.h"
#include "Export_Utility.h"



CKickBoardMonster_Attack::CKickBoardMonster_Attack()
{


}

CKickBoardMonster_Attack::~CKickBoardMonster_Attack()
{
}

void CKickBoardMonster_Attack::Initialize(CMonster* _Monster)
{
	m_pHost = _Monster;
	m_fMinFrame = 3.0f;
	m_fMaxFrame = 4.0f;
	m_fCurFrame = m_fMinFrame;

	m_iHor = 5;

}

CMonsterState* CKickBoardMonster_Attack::Update(CMonster* Monster, const float& fDetltaTime)
{

	switch (m_eAttack)
	{
	case CKickBoardMonster_Attack::READY:
	{
		m_fTick += fDetltaTime;

		if (m_fTick > 1.f)
		{
			++m_fCurFrame;
			m_eAttack = JUMP;
			m_fTick = 0.f;
		}
		break;
	}

	case CKickBoardMonster_Attack::JUMP:
	{
		m_fTick += fDetltaTime;
		m_pHost->Chase_Target(fDetltaTime);
		if (m_fTick >= 1.f)
		{

			++m_fCurFrame;

			m_eAttack = JUMPEND;
			m_fTick = 0.f;
		}
		break;
	}

	case CKickBoardMonster_Attack::JUMPEND:
	{
		if (m_bJump)
		{



			m_pHost->Get_RigidBodyCom()->Set_Force(_vec3{ -m_pHost->Get_Length(),40.f,0.f });
			m_bJump = false;
		}
		m_fTick += fDetltaTime;

		if (m_fTick >= 1.f)
		{
			//TODO 만약 점프 후 플레이어가 공격범위에 없을시 추격 상태로 변경
			if (!m_pHost->ChaseCatch())
			{
				return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(1);
			}

			//TODO 공격 후 패트롤 상태로 변경
// 			else
// 			{
// 				return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(3);
// 			}
			m_fCurFrame = 2;
			m_eAttack = READY;
			m_bJump = true;
			m_fTick = 0.f;
		}
		break;
	}
	}

	if (m_fCurFrame > m_fMaxFrame)
	{
		m_fCurFrame = m_fMinFrame;
	}

	return nullptr;
}

void CKickBoardMonster_Attack::Release(CMonster* _Monster)
{

}

void CKickBoardMonster_Attack::LateUpdate(CMonster* _Monster)
{
}

void CKickBoardMonster_Attack::Render(CMonster* _Monster)
{

}
