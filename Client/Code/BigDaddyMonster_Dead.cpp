#include "stdafx.h"
#include "BigDaddyMonster_Dead.h"
#include "SYTex.h"
#include "Texture.h"

CBigDaddyMonster_Dead::CBigDaddyMonster_Dead()
{

}

CBigDaddyMonster_Dead::~CBigDaddyMonster_Dead()
{
}

void CBigDaddyMonster_Dead::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 1.f;
	m_fMaxFrame = 4.f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 4;

}

CMonsterState* CBigDaddyMonster_Dead::Update(CMonster* Monster, const float& fDetltaTime)
{
	m_fTick += fDetltaTime;

	if (m_fTick >= 0.5f)
	{
		++m_fCurFrame;
		m_fTick = 0;
	}

	if (m_fCurFrame > m_fMaxFrame)
		m_fCurFrame = m_fMinFrame;

		return nullptr;
}

void CBigDaddyMonster_Dead::Release(CMonster* _Monster)
{
}

void CBigDaddyMonster_Dead::LateUpdate(CMonster* _Monster)
{
}

void CBigDaddyMonster_Dead::Render(CMonster* _Monster)
{
}
