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
	m_fMinFrame = 2.0f;
	m_fMaxFrame = 3.0f;
	m_fCurFrame = m_fMinFrame;

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

		if (m_fTick >= 1.f)
		{
			m_fCurFrame = 1;
			m_eAttack = JUMPEND;
			m_fTick = 0.f;
		}
		break;
	}

	case CKickBoardMonster_Attack::JUMPEND:
	{
		m_fTick += fDetltaTime;

		if (m_fTick >= 1.f)
		{
			//TODO 만약 점프 후 플레이어가 공격범위에 없을시 추격 상태로 변경
			if (!m_pHost->ChaseCatch())
				return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(1);

			//TODO 공격 후 패트롤 상태로 변경
			else
			{
				return dynamic_cast<CKickBoardMonster*>(m_pHost)->Get_State(3);
			}
			++m_fCurFrame;
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
	if (m_pHost->Get_Info().bHit)
	{
		m_pHost->Get_TextureCom()->Render_Textrue(3);
	}
	else
	{
		m_pHost->Get_TextureCom()->Render_Textrue(2);
	}

	m_pHost->Get_BufferCom()->Render_Buffer(m_fCurFrame, 1);
}
