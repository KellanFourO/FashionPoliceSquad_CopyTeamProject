#include "stdafx.h"
#include "Stage1Boss_Jump.h"
#include "SYTex.h"
#include "Texture.h"
#include "RigidBody.h"
#include <random>

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

	m_fMinFrame = 2;
	m_fMaxFrame = 4;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 1;
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
				m_fCurFrame = 3;
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
				m_fCurFrame = 4;
				m_eJumpState = JUMP_END;
				m_fTick = 0;
			}

			if (m_pHost->m_bStart)
			{
				m_pHost->Get_RigidBodyCom()->Set_Force(_vec3{ 0.f, 100.f, 50.f});
				m_pHost->m_bStart = FALSE;
			}
			else
			{
				if (m_bJump)
				{
					m_pHost->Get_RigidBodyCom()->Set_Force(_vec3{ 0.f,100.f,0.f });
					m_bJump = false;
				}
			}
		}

		break;

	case CStage1Boss_Jump::JUMP_END:
		{
			m_fTick += fDetltaTime;

			random_device rd;
			mt19937 gen(rd());

			uniform_int_distribution<int> distribution(2, 3); // 랜덤 시작 부터 마지막

			int iRandomValue = distribution(gen);

 			return m_pHost->Get_State(iRandomValue);

		}

		break;
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
