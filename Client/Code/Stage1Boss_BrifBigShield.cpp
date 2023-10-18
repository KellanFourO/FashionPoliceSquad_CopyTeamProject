#include "stdafx.h"
#include "Stage1Boss_BrifBigShield.h"
#include <random>
#include "Export_Utility.h"
#include "SYTex.h"
#include "Texture.h"
#include "BossBigDaddy.h"
#include "BossBigDaddy2.h"
#include "BossBigDaddy3.h"
#include "BossBigDaddy4.h"
#include "Stage1Boss.h"

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
	CStage1Boss::m_bBossPhase2 = true;
	m_pHost = _Monster;

	
	//BossbigDaddy 왼쪽1
	CBossBigDaddy* BossBigDaddy = CBossBigDaddy::Create(m_pHost->Get_GraphicDev(),0);
	BossBigDaddy->Set_ObjectTag(OBJECTTAG::MONSTER);
	Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::MONSTER, BossBigDaddy);
	//BossbigDaddy 왼쪽2
	CBossBigDaddy2* BossBigDaddy2 = CBossBigDaddy2::Create(m_pHost->Get_GraphicDev(),1);
	
	BossBigDaddy2->Set_ObjectTag(OBJECTTAG::MONSTER);
	Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::MONSTER, BossBigDaddy2);
	//BossbigDaddy 오른쪽3
	CBossBigDaddy3* BossBigDaddy3 = CBossBigDaddy3::Create(m_pHost->Get_GraphicDev(),2);
	BossBigDaddy3->Set_ObjectTag(OBJECTTAG::MONSTER);
	Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::MONSTER, BossBigDaddy3);
	//BossbigDaddy 오른쪽4
	CBossBigDaddy4* BossBigDaddy4 = CBossBigDaddy4::Create(m_pHost->Get_GraphicDev(),3);
	BossBigDaddy4->Set_ObjectTag(OBJECTTAG::MONSTER);
	Management()->Get_Layer(LAYERTAG::GAMELOGIC)->Add_GameObject(OBJECTTAG::MONSTER, BossBigDaddy4);
	
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
		m_fTick += fDetltaTime;
		if (m_fTick > 1)
		{
			++m_fCurFrame;
			if (m_fCurFrame > m_fMaxFrame)
			{
				m_fCurFrame = m_fMinFrame;
			}
			m_fTick = 0.f;
		}
		
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
		m_fTick += fDetltaTime;
		if (m_fTick > 1)
		{
			++m_fCurFrame;
			if (m_fCurFrame > m_fMaxFrame)
			{
				m_fCurFrame = m_fMinFrame;
			}
			m_fTick = 0.f;
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
		m_fTick += fDetltaTime;
		if (m_fTick > 1)
		{
			++m_fCurFrame;
			if (m_fCurFrame > m_fMaxFrame)
			{
				m_fCurFrame = m_fMinFrame;
			}
			m_fTick = 0.f;
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
			CStage1Boss::m_bBossPhase2 = false;
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
