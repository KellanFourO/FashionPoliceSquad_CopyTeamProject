#include "stdafx.h"
#include "Stage1Boss_Jump.h"
#include "SYTex.h"
#include "Texture.h"
#include "RigidBody.h"
#include <random>
#include "JumpShockWaveEffect.h"
#include "Export_Utility.h"
CStage1Boss_Jump::CStage1Boss_Jump()
{

}

CStage1Boss_Jump::~CStage1Boss_Jump()
{
}

void CStage1Boss_Jump::Initialize(CMonster* _Monster)
{
	m_pHost = _Monster;


	//TODO 페이즈에 따라 점프 속도 증가 할 예정
	switch (m_ePhase)
	{
	case Engine::BOSSPHASE::PHASE_1:
		m_fJumpSpeed = 125.f;
		break;
	case Engine::BOSSPHASE::PHASE_2:
		m_fJumpSpeed = 175.f;
		m_pHost->Get_RigidBodyCom()->Set_Heavy(15.0f);
		m_fAgainTime = 0.85f;
		m_fJumpStaytime = 0.75f;
		break;
	case Engine::BOSSPHASE::PHASE_3:
		m_fJumpSpeed = 220.f;
		m_pHost->Get_RigidBodyCom()->Set_Heavy(20.0f);
		m_fAgainTime = 0.55f;
		m_fJumpStaytime = 0.45f;
		break;
	}

	m_fMinFrame = 2;
	m_fMaxFrame = 4;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 1;
	m_eJumpState = JUMP_READY;

	_float m_fTop = 185.f;
	_float m_fBottom = 50.f;

	_float m_fLeft = 42.5f;
	_float m_fRight = 142.5f;

	vJumpPoint[0] = { m_fLeft,	20.f,	m_fTop	};
	vJumpPoint[1] = { m_fRight,	20.f,	m_fTop	};
	vJumpPoint[2] = { m_fLeft,	20.f, m_fBottom	};
	vJumpPoint[3] = { m_fRight, 20.f, m_fBottom };

	random_device rd;
	mt19937 gen(rd());

	uniform_int_distribution<int> distribution(0, 3); // 랜덤 시작 부터 마지막
	m_iRandomIndex = distribution(gen);

	while (true)
	{
		random_device rd;
		mt19937 gen(rd());

		uniform_int_distribution<int> distribution(0, 3);

		m_iRandomIndex = distribution(gen);

		if (static_cast<CStage1Boss*>(m_pHost)->Get_OriginIndex() != m_iRandomIndex)
		{
			break;
		}
	}

	static_cast<CStage1Boss*>(m_pHost)->Set_OriginIndex(m_iRandomIndex);
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
				++m_fCurFrame;
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

			if (m_bJump)
				{
					m_pHost->Get_RigidBodyCom()->Set_Force(_vec3{ 0.f,100.f,0.f});

					m_bJump = false;
				}
			else
			{
				_vec3 vPos = m_pHost->Get_Transform()->m_vInfo[INFO_POS];
				_vec3 vDir = vJumpPoint[static_cast<CStage1Boss*>(m_pHost)->Get_OriginIndex()] - vPos;
				D3DXVec3Normalize(&vDir, &vDir);

				m_pHost->Get_Transform()->Move_Pos(&vDir, fDetltaTime, m_fJumpSpeed);
			}
		}

		break;

	case CStage1Boss_Jump::JUMP_END:
		{
			m_fTick += fDetltaTime;

			if (m_bEffect)
			{
				_vec3 vHostPos = m_pHost->Get_Transform()->m_vInfo[INFO_POS];
				_vec3 vCreatePos = {vHostPos.x, vHostPos.y - 20.f, vHostPos.z};
				CGameObject* pShockWave = CJumpShockWaveEffect::Create(m_pHost->Get_GraphicDev(), vCreatePos);
				Management()->Get_Layer(LAYERTAG::UI)->Add_GameObject(OBJECTTAG::EFFECT,pShockWave);
				m_bEffect = false;
			}

			if (m_fTick > m_fAgainTime)
			{
				random_device rd;
				mt19937 gen(rd());

				uniform_int_distribution<int> distribution(2, 3); // 랜덤 시작 부터 마지막

				int iRandomValue = distribution(gen);
				//return dynamic_cast<CStage1Boss*>(m_pHost)->Get_State(2);
				return dynamic_cast<CStage1Boss*>(m_pHost)->Get_State(iRandomValue);
				m_fTick = 0;
			}
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
	m_bEffect = true;
}

void CStage1Boss_Jump::Render(CMonster* _Monster)
{
}

void CStage1Boss_Jump::ShockWave()
{


	if (m_eJumpState == JUMP_END)
	{
		_vec3 vRadius = { m_fShockRadius, m_fShockRadius , m_fShockRadius };
		_vec3 vBossPos = m_pHost->Get_TransformCom()->m_vInfo[INFO_POS];


	}
}
