#include "stdafx.h"
#include "BossBigDaddy_Idle.h"
#include "SYTex.h"
#include "Texture.h"
#include "Stage1Boss.h"

#include "Stage1Boss_BrifBigShield.h"


CBossBigDaddy_Idle::CBossBigDaddy_Idle()
{

}

CBossBigDaddy_Idle::~CBossBigDaddy_Idle()
{
}

void CBossBigDaddy_Idle::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 1.0f;
	m_fMaxFrame = 1.0f;
	m_fCurFrame = m_fMinFrame;

	m_iVer = 1;
}

CMonsterState* CBossBigDaddy_Idle::Update(CMonster* Monster, const float& fDetltaTime)
{
	if (m_pHost->ChaseCatch())
	{
		//todo 대상이 공격범위 안에 있을시

		switch (m_pHost->Get_MonsterIndex())
		{
			case 0:
			{
				return dynamic_cast<CBossBigDaddy*>(m_pHost)->Get_State(1); // Attack;
				break;
			}
			case 1:
			{
				return dynamic_cast<CBossBigDaddy2*>(m_pHost)->Get_State(1); // Attack;
				break;
			}
			case 2:
			{
				return dynamic_cast<CBossBigDaddy3*>(m_pHost)->Get_State(1); // Attack;
				break;
			}
			case 3:
			{
				return dynamic_cast<CBossBigDaddy4*>(m_pHost)->Get_State(1); // Attack;
				break;
			}
		
		}

		
	}

	return nullptr;
}

void CBossBigDaddy_Idle::Release(CMonster* _Monster)
{
}

void CBossBigDaddy_Idle::LateUpdate(CMonster* _Monster)
{
}

void CBossBigDaddy_Idle::Render(CMonster* _Monster)
{
}
