#include "stdafx.h"
#include "Stage1Boss_Dead.h"
#include "Texture.h"

CStage1Boss_Dead::CStage1Boss_Dead()
\
{

}

CStage1Boss_Dead::~CStage1Boss_Dead()
{
}

void CStage1Boss_Dead::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);
	m_iVer = 1;

	m_fMinFrame = 1;
	m_fMaxFrame = 5;
	m_fCurFrame = m_fMinFrame;

	SoundMgr()->PlaySoundW(L"BossDead.wav",SOUND_BOSS, 1.f);

}

CMonsterState* CStage1Boss_Dead::Update(CMonster* Monster, const float& fDetltaTime)
{
	m_fTick += fDetltaTime;

	if (m_fTick > 0.5f)
	{
		++m_fCurFrame;

		m_fTick = 0;
	}

	if(m_fCurFrame > m_fMaxFrame)
		m_fCurFrame = m_fMinFrame;


	return nullptr;
}

void CStage1Boss_Dead::LateUpdate(CMonster* _Monster)
{
}

void CStage1Boss_Dead::Release(CMonster* _Monster)
{
	SoundMgr()->StopSound(SOUND_BOSS);
}

void CStage1Boss_Dead::Render(CMonster* _Monster)
{

}
