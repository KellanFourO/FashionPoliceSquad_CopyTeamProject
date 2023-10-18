#include "stdafx.h"
#include "BossBigDaddy_Dead.h"
#include "SYTex.h"
#include "Texture.h"

CBossBigDaddy_Dead::CBossBigDaddy_Dead()
{

}

CBossBigDaddy_Dead::~CBossBigDaddy_Dead()
{
}

void CBossBigDaddy_Dead::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 1.f;
	m_fMaxFrame = 4.f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 4;

}

CMonsterState* CBossBigDaddy_Dead::Update(CMonster* Monster, const float& fDetltaTime)
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

void CBossBigDaddy_Dead::Release(CMonster* _Monster)
{
}

void CBossBigDaddy_Dead::LateUpdate(CMonster* _Monster)
{
}

void CBossBigDaddy_Dead::Render(CMonster* _Monster)
{
}
