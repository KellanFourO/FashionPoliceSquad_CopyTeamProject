#include "stdafx.h"
#include "Stage1Boss_BrifBigShield.h"
#include <random>
#include "Export_Utility.h"
#include "SYTex.h"
#include "Texture.h"

bool CStage1Boss_BrifBigShield::m_bcheck1 = false;
bool CStage1Boss_BrifBigShield::m_bcheck2 = false;
bool CStage1Boss_BrifBigShield::m_bcheck3 = false;
bool CStage1Boss_BrifBigShield::m_bcheck4 = false;
bool CStage1Boss_BrifBigShield::m_bLeftDead = false;
bool CStage1Boss_BrifBigShield::m_bRightDead = false;
bool CStage1Boss_BrifBigShield::m_bAllDead = false;

CStage1Boss_BrifBigShield::CStage1Boss_BrifBigShield()
{

}

CStage1Boss_BrifBigShield::~CStage1Boss_BrifBigShield()
{
}

void CStage1Boss_BrifBigShield::Initialize(CMonster* _Monster)
{
	m_pHost = _Monster;

	switch (m_ePhase)
	{
	case Engine::BOSSPHASE::PHASE_1:
		break;
	case Engine::BOSSPHASE::PHASE_2:
		//BossbigDaddy 왼쪽1
		 
		//BossbigDaddy 왼쪽2
		 
		//BossbigDaddy 오른쪽3

		//BossbigDaddy 오른쪽4
		break;
	case Engine::BOSSPHASE::PHASE_3:
		break;

	}
	m_eBossDaddyDie = ALL_LIVE;
	m_fMinFrame = 4;
	m_fMaxFrame = 5;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 2;
}

CMonsterState* CStage1Boss_BrifBigShield::Update(CMonster* Monster, const float& fDetltaTime)
{
	
	m_pHost->Chase_Target(fDetltaTime);
	switch (m_eBossDaddyDie)
	{
	case CStage1Boss_BrifBigShield::ALL_LIVE:
		{
		if (m_bcheck1 && m_bcheck2 &&!m_bLeftDead)
			{
				m_eBossDaddyDie = RIGHT_LIVE;
				m_bLeftDead = true;
			}
		else if (m_bcheck3 && m_bcheck4 && !m_bRightDead)
			{
				m_eBossDaddyDie = LEFT_LIVE;
				m_bRightDead = true;
			}
		}
		break;
	case CStage1Boss_BrifBigShield::LEFT_LIVE:
		{
		if (m_bRightDead)
			{
				m_fMinFrame = 4;
				m_fMaxFrame = 5;
				m_fCurFrame = m_fMinFrame;
				m_iVer = 4;
			}
		if (m_bcheck1 && m_bcheck2 && !m_bLeftDead)
			{
				m_bLeftDead = true;
				m_eBossDaddyDie = ALL_DEAD;
			}
			
		}
		break;
	case CStage1Boss_BrifBigShield::RIGHT_LIVE:
		{
		if (m_bLeftDead)
			{
				m_fMinFrame = 4;
				m_fMaxFrame = 5;
				m_fCurFrame = m_fMinFrame;
				m_iVer = 3;
			}
		if (m_bcheck3 && m_bcheck4 && !m_bRightDead)
		{
			m_bRightDead = true;
			m_eBossDaddyDie = ALL_DEAD;
		}
		}
		break;
	case CStage1Boss_BrifBigShield::ALL_DEAD:
		{
			m_bAllDead = true;
			m_fTick += fDetltaTime;
			if (m_fTick > m_fAgainTime)
			{
				random_device rd;
				mt19937 gen(rd());

				uniform_int_distribution<int> distribution(2, 3); // 랜덤 시작 부터 마지막

				int iRandomValue = distribution(gen);
				return dynamic_cast<CStage1Boss*>(m_pHost)->Get_State(iRandomValue);
				m_fTick = 0;
			}
		}
		break;
	}

	return nullptr;
}

void CStage1Boss_BrifBigShield::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_BrifBigShield::Release(CMonster* _Monster)
{
}

void CStage1Boss_BrifBigShield::Render(CMonster* _Monster)
{
}
