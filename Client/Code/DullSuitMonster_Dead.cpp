#include "stdafx.h"
#include "DullSuitMonster_Dead.h"
#include "SYTex.h"
#include "Texture.h"

CDullSuitMonster_Dead::CDullSuitMonster_Dead()
{

}

CDullSuitMonster_Dead::~CDullSuitMonster_Dead()
{
}

void CDullSuitMonster_Dead::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 1.0f;
	m_fMaxFrame = 2.0f;
	m_fCurFrame = m_fMinFrame;

	m_iVer = 3;
	SoundMgr()->PlaySound(L"DullSuitDead.wav",SOUND_MONSTER,0.5);
}

CMonsterState* CDullSuitMonster_Dead::Update(CMonster* Monster, const float& fDetltaTime)
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

void CDullSuitMonster_Dead::LateUpdate(CMonster* _Monster)
{
}

void CDullSuitMonster_Dead::Release(CMonster* _Monster)
{
	SoundMgr()->StopSound(SOUND_MONSTER);
}

void CDullSuitMonster_Dead::Render(CMonster* _Monster)
{
}
