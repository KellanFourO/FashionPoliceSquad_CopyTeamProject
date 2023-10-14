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
	m_pHost = (_Monster);

	m_fMinFrame = 2.f;
	m_fMaxFrame = 3.f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 5;
}

CMonsterState* CStage1Boss_Idle::Update(CMonster* Monster, const float& fDetltaTime)
{

	m_fTick += fDetltaTime;

	if (m_fTick >= 0.5f)
	{
		++m_fCurFrame;
		m_fTick = 0.f;
	}

	if (m_fCurFrame > m_fMaxFrame)
	{
		m_fCurFrame = m_fMinFrame;
	}

	if (m_pHost->Get_Start() && m_fCurFrame == m_fMinFrame)
	{
		//TODO 여기서 시작 점프 상태로 변경

		return dynamic_cast<CStage1Boss*>(m_pHost)->Get_State(1);
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

}
