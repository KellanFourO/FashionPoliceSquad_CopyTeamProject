#include "stdafx.h"
#include "..\Header\BrifMonsterBig_DEAD2.h"

CBrifMonsterBig_DEAD2::CBrifMonsterBig_DEAD2()
{

}

CBrifMonsterBig_DEAD2::~CBrifMonsterBig_DEAD2()
{
}

void CBrifMonsterBig_DEAD2::Initialize(CMonster* _Monster)
{
	m_fCurFrame = 11.f;
	m_fBehaviorTime = 0.f;
	m_fAnimateTime = 0.f;
}

CMonsterState* CBrifMonsterBig_DEAD2::Update(CMonster* Monster, const float& fDetltaTime)
{
	m_fAnimateTime += fDetltaTime;
	if (m_fAnimateTime >= 1.f)
	{
		return dynamic_cast<CBrifMonsterBig*>(m_pHost)->Get_State(2);
	}
	Monster->Set_Frame(4, 4, m_fCurFrame);
	return nullptr;
}

void CBrifMonsterBig_DEAD2::Release(CMonster* _Monster)
{
}

void CBrifMonsterBig_DEAD2::LateUpdate(CMonster* _Monster)
{
}

void CBrifMonsterBig_DEAD2::Render(CMonster* _Monster)
{
}
