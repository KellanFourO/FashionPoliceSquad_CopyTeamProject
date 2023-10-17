#include "stdafx.h"
#include "BossBigDaddy_Idle.h"
#include "SYTex.h"
#include "Texture.h"

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
	if (m_pHost->Detect() || m_pHost->Get_Info().bHit)
	{
		//TODO �÷��̾ �������� �ȿ� �����ų� �ǰݽ� �߰�. Chase ���·� ����
		return dynamic_cast<CBossBigDaddy*>(m_pHost)->Get_State(1);
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
