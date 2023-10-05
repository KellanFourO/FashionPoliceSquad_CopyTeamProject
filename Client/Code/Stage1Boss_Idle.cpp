#include "stdafx.h"
#include "Stage1Boss_Idle.h"
#include "SYTex.h"
#include "Texture.h"
#include "Engine_Define.h"

CStage1Boss_Idle::CStage1Boss_Idle()
{

}

CStage1Boss_Idle::~CStage1Boss_Idle()
{
}

void CStage1Boss_Idle::Initialize(CMonster* _Monster)
{
	m_pHost = dynamic_cast<CStage1Boss*>(_Monster);
}

CMonsterState* CStage1Boss_Idle::Update(CMonster* Monster, const float& fDetltaTime)
{

	m_fTick += fDetltaTime;


	if (m_fTestTick >= 5)
	{
		m_bStart = true;
	}

	if (m_fTick >= 1)
	{
		m_iIdleStart++;
		m_fTick = 0;

		if (m_iIdleStart > m_iIdleEnd)
		{
			m_iIdleStart = 2;

			m_fTestTick++;
		}
	}

	if (m_bStart)
	{
		//TODO 여기서 시작 점프 상태로 변경

		return m_pHost->Get_State(1);
	}

	return nullptr;
}

void CStage1Boss_Idle::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_Idle::Release(CMonster* _Monster)
{
}

void CStage1Boss_Idle::Render(CMonster* _Monster)
{
	m_pHost->Get_TextureCom()->Render_Textrue(0);
	m_pHost->Get_BufferCom()->Render_Buffer(m_iIdleStart, 5);
}
