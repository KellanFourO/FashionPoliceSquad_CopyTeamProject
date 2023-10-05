#include "stdafx.h"
#include "..\Header\KickBoard_DEAD.h"


//#include "..\Header\MonsterState_DEAD_Walk.h"
//#include "..\Header\MonsterState_DEAD_Attack.h"

CKickBoard_DEAD::CKickBoard_DEAD()
{

}

CKickBoard_DEAD::~CKickBoard_DEAD()
{
}

void CKickBoard_DEAD::Initialize(CMonster* _Monster)
{
	m_fMinFrame = 12.0f;
	m_fMaxFrame = 14.0f;
	m_fCurFrame = m_fMinFrame;

	m_fBehaviorTime = 0.f;
	m_fAnimateTime = 0.f;
}

CMonsterState* CKickBoard_DEAD::Update(CMonster* Monster, const float& fDetltaTime)
{
	m_fAnimateTime += fDetltaTime;
	m_fBehaviorTime += fDetltaTime;
	if (m_fAnimateTime >= 0.3f)
	{
		++m_fCurFrame;
		m_fAnimateTime = 0;
	}

	if (m_fCurFrame >= m_fMaxFrame) {
		m_fCurFrame = m_fMinFrame;
	}
	Monster->Set_Frame(4, 4, m_fCurFrame);
	return nullptr;
}

void CKickBoard_DEAD::LateUpdate(CMonster* _Monster)
{
}

void CKickBoard_DEAD::Release(CMonster* _Monster)
{
}

void CKickBoard_DEAD::Render(CMonster* _Monster)
{
}
