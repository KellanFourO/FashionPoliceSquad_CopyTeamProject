#include "stdafx.h"
#include "KickBoardMonster_Dead.h"
#include "SYTex.h"
#include "Texture.h"

CKickBoardMonster_Dead::CKickBoardMonster_Dead()
{

}

CKickBoardMonster_Dead::~CKickBoardMonster_Dead()
{
}

void CKickBoardMonster_Dead::Initialize(CMonster* _Monster)
{
	m_pHost = (_Monster);

	m_fMinFrame = 1.0f;
	m_fMaxFrame = 2.0f;
	m_fCurFrame = m_fMinFrame;
	m_iVer = 4;

}

CMonsterState* CKickBoardMonster_Dead::Update(CMonster* Monster, const float& fDetltaTime)
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

void CKickBoardMonster_Dead::LateUpdate(CMonster* _Monster)
{
}

void CKickBoardMonster_Dead::Release(CMonster* _Monster)
{
}

void CKickBoardMonster_Dead::Render(CMonster* _Monster)
{
}
