#include "stdafx.h"
#include "BigDaddyMonster_Idle.h"
#include "SYTex.h"
#include "Texture.h"

CBigDaddyMonster_Idle::CBigDaddyMonster_Idle()
{

}

CBigDaddyMonster_Idle::~CBigDaddyMonster_Idle()
{
}

void CBigDaddyMonster_Idle::Initialize(CMonster* _Monster)
{	
	m_pHost = (_Monster);

	m_fMinFrame = 1.0f;
	m_fMaxFrame = 1.0f;
	m_fCurFrame = m_fMinFrame;
}

CMonsterState* CBigDaddyMonster_Idle::Update(CMonster* Monster, const float& fDetltaTime)
{
	if (m_pHost->Detect() || m_pHost->Get_Info().bHit)
	{
		//TODO 플레이어가 일정범위 안에 들어오거나 피격시 추격. Chase 상태로 변경
		return dynamic_cast<CBigDaddyMonster*>(m_pHost)->Get_State(1);
	}
	
	return nullptr;

}

void CBigDaddyMonster_Idle::Release(CMonster* _Monster)
{
}

void CBigDaddyMonster_Idle::LateUpdate(CMonster* _Monster)
{
}

void CBigDaddyMonster_Idle::Render(CMonster* _Monster)
{
	if (m_pHost->Get_Info().bHit)
	{
		m_pHost->Get_TextureCom()->Render_Textrue(1);
	}
	else
	{
		m_pHost->Get_TextureCom()->Render_Textrue(0);
	}
	
	m_pHost->Get_BufferCom()->Render_Buffer(m_fCurFrame, m_fMaxFrame);
}
