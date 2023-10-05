#include "stdafx.h"
#include "Stage1Boss_Jump.h"
#include "SYTex.h"
#include "Texture.h"
#include "RigidBody.h"


CStage1Boss_Jump::CStage1Boss_Jump()
{

}

CStage1Boss_Jump::~CStage1Boss_Jump()
{
}

void CStage1Boss_Jump::Initialize(CMonster* _Monster)
{
	m_pHost = dynamic_cast<CStage1Boss*>(_Monster);


	//TODO 페이즈에 따라 점프 속도 증가 할 예정
	switch (m_ePhase)
	{
	case Engine::BOSSPHASE::PHASE_1:
		break;
	case Engine::BOSSPHASE::PHASE_2:
		break;
	case Engine::BOSSPHASE::PHASE_3:
		break;
	}
}

CMonsterState* CStage1Boss_Jump::Update(CMonster* Monster, const float& fDetltaTime)
{

	switch (m_eJumpState)
	{
	case CStage1Boss_Jump::JUMP_READY:
		{
			m_fTick += fDetltaTime;

			if (m_fTick > 1)
			{
				m_iJumpStart = 3;
				m_eJumpState = JUMP_START;
				m_bJump = true;
				m_fTick = 0;
			}
		}
		break;

	case CStage1Boss_Jump::JUMP_START:
		{
			m_fTick += fDetltaTime;

			if (m_fTick > m_fJumpStaytime)
			{
				m_iJumpStart = 2;
				m_eJumpState = JUMP_END;
				m_fTick = 0;
			}

			if (m_bJump)
			{
				m_pHost->Get_RigidBodyCom()->Set_Force(_vec3{ 0.f,100.f,0.f });
				m_bJump = false;
			}
		}

		break;

	case CStage1Boss_Jump::JUMP_END:
		{
			m_fTick += fDetltaTime;

			if (m_fTick > m_fAgainTime)
			{
				m_iJumpStart = 2;
				m_eJumpState = JUMP_READY;
				m_fTick = 0;


				return m_pHost->Get_State(2);
			}

		}

		break;
	}


	if (m_bChange)
	{
		//TODO 페이즈에따라 점프횟수로 제한을 하거나, 일정 시간이 되면 상태를 변경시킬 수 있음.
		//TODO 페이즈 증가에 따라 시간에 속도가 증가하는것도 좋을듯

		return m_pHost->Get_State(2); // !THROWSINGLE

	}

	return nullptr;
}

void CStage1Boss_Jump::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_Jump::Release(CMonster* _Monster)
{
}

void CStage1Boss_Jump::Render(CMonster* _Monster)
{
	m_pHost->Get_TextureCom()->Render_Textrue(0);
	m_pHost->Get_BufferCom()->Render_Buffer(m_iJumpStart, 1);
}

void CStage1Boss_Jump::ShockWave()
{
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM));
	NULL_CHECK(pPlayerTransCom);



	if (m_eJumpState == JUMP_END)
	{
		_vec3 vRadius = { m_fShockRadius, m_fShockRadius , m_fShockRadius };
		_vec3 vBossPos = m_pHost->Get_TransformCom()->m_vInfo[INFO_POS];


	}
}
